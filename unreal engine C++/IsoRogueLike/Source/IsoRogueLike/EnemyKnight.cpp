// Fill out your copyright notice in the Description page of Project Settings.



#include "EnemyKnight.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
#include "EnemyHealthBar.h"
#include "GameCamera.h"

// Sets default values
AEnemyKnight::AEnemyKnight()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
}

// Called when the game starts or when spawned
void AEnemyKnight::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> Player;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), Player);
	if (Player.Num() > 0)
	{
		PlayerActor = Player[0];
	}
	
	StartLocation = this->GetActorLocation();
	KnightHealthCurrent = KnightHealthStart;
	Mass = 3.0;
	EnemyHealthWidget = Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()));
	HealthBar = Cast<UEnemyHealthBar>(EnemyHealthWidget->GetUserWidgetObject());
	CollisionComponent = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));
	KnightController = Cast<AMainAIController>(GetController());
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyKnight::OnOverlapBegin);
	}

	if (HealthBar) {
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString(EnemyHealthWidget->GetWidgetClass()->GetDisplayNameText().ToString()));
	}
	ActuallyFireTimerDelegate.BindUFunction(this, FName("Fire"));
	CanFireTimerDelegate.BindUFunction(this, FName("ResetCanFire"));
	ShotSpawn = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("ProjectileSpawnPoint")));
}

void AEnemyKnight::ProcessStatus(KnightStatus State)
{
	switch (State)
	{
		case KnightStatus::Idle:
			Currentstatus = IdleStateHandle();
		break;
		
		case KnightStatus::Moving:
			Currentstatus = MovingStateHandle();
		break;
		
	default:
		break;
	}
}

KnightStatus AEnemyKnight::IdleStateHandle()
{
	KnightController->StopMovement();
	if (dist > AggroRange)
	{
		return KnightStatus::Moving;
	}	
	
	return KnightStatus::Idle;
}

KnightStatus AEnemyKnight::MovingStateHandle()
{
	if (dist < AggroRange)
	{
		return KnightStatus::Idle;
	}
	return KnightStatus::Moving;
}
void AEnemyKnight::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AProjectile::StaticClass()))
	{
		if (KnightHealthCurrent - 1 <= 0)
		{
			GetWorld()->DestroyActor(this);
		}
		else{
			KnightHealthCurrent -= 1;
			FVector Direction = this->GetActorLocation() - OtherActor->GetActorLocation();
			AddImpact(Direction, 1000);
		}
	}
}
void AEnemyKnight::SpawnProjectile()
{
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.bNoFail = true;	
	Projectile = World->SpawnActor<AProjectile>(ShotSpawn->GetComponentLocation(), this->GetActorRotation(), SpawnParameters);
	if (ActuallyFireTimerDelegate.IsBound())
	{
		GetWorld()->GetTimerManager().SetTimer(ActuallyFireTimerHandle, ActuallyFireTimerDelegate, 0.5, false, DelayBetweenShowingAndFiring);
	}
}

void AEnemyKnight::Fire()
{
	Projectile->FireInDirection(this->GetActorForwardVector(), this);	
	Projectile = nullptr;
}
float AEnemyKnight::GetRotationSpeed()
{
	if (Projectile) { return RotationSpeedProjectile; }
	return RotationSpeedNormal;
}
void AEnemyKnight::ResetCanFire()
{
	CanFire = true;
}
void AEnemyKnight::AddImpact(FVector Direction, float Force)
{
	FVector Dir = Direction;
	Dir.Normalize();

	Impact += Dir.GetSafeNormal() * Force / Mass;
}
void AEnemyKnight::Tick(float DeltaTime)
{ 
	Super::Tick(DeltaTime);
	ProcessStatus(Currentstatus);

	if (Impact.Size() > 0.4)
	{
		this->AddMovementInput(Impact * DeltaTime);
		Impact = UKismetMathLibrary::VLerp(Impact, FVector::ZeroVector, 6 * DeltaTime);
	}

	if (Currentstatus != KnightStatus::Moving)
	{
		Moving = false;
	}else
	{
		Moving = true;
	}

	FRotator RotToPlayer = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), PlayerActor->GetActorLocation());
	RotToPlayer.Pitch = 0;
	this->SetActorRotation(UKismetMathLibrary::RLerp(this->GetActorRotation(),RotToPlayer,GetRotationSpeed() * DeltaTime,false));
	if (HealthBar)
	{
		float HealthBarScale = KnightHealthCurrent / KnightHealthStart;
		HealthBar->UpdateHealthbar(HealthBarScale);
		AActor* GameCam = UGameplayStatics::GetActorOfClass(GetWorld(), AGameCamera::StaticClass());
		//EnemyHealthWidget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(EnemyHealthWidget->GetComponentRotation().Vector(), GameCam->GetActorRotation().Vector()));
	}
	if (CanFireTimerDelegate.IsBound() && CanFire)
	{
		CanFire = false;
		SpawnProjectile();
		GetWorld()->GetTimerManager().SetTimer(CanFireTimerHandle, CanFireTimerDelegate, 0.5, false, DelayBetweenShowingNewShots);
	}
	if (Projectile)
	{
		Projectile->SetActorLocation(ShotSpawn->GetComponentLocation());
	}
}
