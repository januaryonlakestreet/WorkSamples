// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "GameCamera.h"
#include "Projectile.h"
#include "PlayerUI.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FClassFinder<UPlayerUI> MenuClassFinder(TEXT("/Game/UI/PlayerUi"));
	MenuClass = MenuClassFinder.Class;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CanFire = true;
	APlayerController* pc = Cast<APlayerController>(this->GetController());
	if (pc)
	{
		pc->bShowMouseCursor = true;
	}

	TArray<AActor*> Cam;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameCamera::StaticClass(), Cam);

	GetWorld()->GetFirstPlayerController()->SetViewTarget(Cam[0]);
	PlayerHud = CreateWidget<UPlayerUI>(GetWorld(), MenuClass);
	if (PlayerHud) {
		PlayerHud->AddToViewport();
	}
	CollisionComponent = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	}
	CanFireTimerDelegate.BindUFunction(this, FName("CanFireReset"));

	ShotSpawn = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("ProjectileSpawnPoint")));
	if (!ShotSpawn) {
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("FFF"));
	}
	//Energy & health setup
	MaxEnergy = 10.f;
	EnergyRechargeRate = 0.005f;
	CurrentEnergy = MaxEnergy;
	ShotEnergyCost = 2.4f;
	
	Mass = 3.0;
	PlayerHealthCurrent = PlayerHealthMax;

}
void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerHealthCurrent -= 1;
	FVector Direction = this->GetActorLocation() - OtherActor->GetActorLocation();
	AddImpact(Direction, 1000);
}
// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Impact.Size() > 0.4)
	{
		this->AddMovementInput(Impact * DeltaTime);
		Impact = UKismetMathLibrary::VLerp(Impact, FVector::ZeroVector, 6 * DeltaTime);
	}

	FRotator Direction = GetLookDirection().Rotation();
	Direction.Pitch = 0;
	Direction.Roll = 0;
	this->SetActorRelativeRotation(Direction);

	if (this->GetLastMovementInputVector().Size() > 0)
	{
		Moving = true;
	}
	else 
	{
		Moving = false;
	}

	float EnergyScale = CurrentEnergy / MaxEnergy;
	PlayerHud->UpdateEnergyBar(EnergyScale);
	float HealthScale = PlayerHealthCurrent / PlayerHealthMax;
	PlayerHud->UpdateHealthBar(HealthScale);
	if (CurrentEnergy < MaxEnergy)
	{
		CurrentEnergy += EnergyRechargeRate;
	}

	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("Forward"), this, &APlayerCharacter::Forward);
	PlayerInputComponent->BindAxis(FName("Sideways"), this, &APlayerCharacter::Sideways);
	PlayerInputComponent->BindAction(FName("Fire"), IE_Pressed, this, &APlayerCharacter::Fire);
	
}

void APlayerCharacter::Forward(float val)
{
	this->AddMovementInput(this->GetActorForwardVector() * MoveSpeed * val);
}
void APlayerCharacter::Sideways(float val)
{
	this->AddMovementInput(this->GetActorRightVector() * MoveSpeed * val);
}
FVector APlayerCharacter::GetLookDirection()
{
	FHitResult Result;
	APlayerController* pc = Cast<APlayerController>(this->GetController());
	pc->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Result);
	FVector HitResults = FVector(Result.Location.X, Result.Location.Y, this->GetActorLocation().Z);
	FVector LookDirection = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), HitResults).Vector();
	
	return LookDirection;
}
void APlayerCharacter::Fire()
{
	if (CanFire && CurrentEnergy > ShotEnergyCost)
	{	
		FVector Dir = GetLookDirection();
		
		CanFire = false;
		UWorld* World = GetWorld();
		

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;	
		SpawnParameters.bNoFail = true;
	
		AProjectile* Projectile = World->SpawnActor<AProjectile>(ShotSpawn->GetComponentLocation(),this->GetActorRotation(),SpawnParameters);
		Projectile->FireInDirection(Dir,this);
		
		if (CanFireTimerDelegate.IsBound())
		{
			GetWorld()->GetTimerManager().SetTimer(CanFireTimerHandle, CanFireTimerDelegate, 0.5, false, 1);
		}
		CurrentEnergy -= ShotEnergyCost;
	}
}

void APlayerCharacter::CanFireReset()
{
	CanFire = true;
}

void APlayerCharacter::AddImpact(FVector Direction, float Force)
{
	FVector Dir = Direction;
	Dir.Normalize();
	
	Impact += Dir.GetSafeNormal() * Force / Mass;
}