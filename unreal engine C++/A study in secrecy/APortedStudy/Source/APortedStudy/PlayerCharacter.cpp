// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameCamera.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MemoryGrid.h"
#include "MemoryGridDeactivationConsole.h"
#include "APortedStudyGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "Interact.h"
#include "Kismet/KismetMathLibrary.h"
#include "InGameUI.h"
#include "UObject/ConstructorHelpers.h"
#include "Match3.h"
#include "UObject/Object.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Rotator.h"
// Sets default values
APlayerCharacter::APlayerCharacter()
{
	ConstructorHelpers::FClassFinder<UInGameUI> MenuClassFinder(TEXT("/Game/UI/PlayerUI"));
	MenuClass = MenuClassFinder.Class;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameCamera::StaticClass(), OutActors);

	if (OutActors.Num() > 0) {
		GetWorld()->GetFirstPlayerController()->SetViewTarget(OutActors[0]);
	}
	UCapsuleComponent* Cap = this->GetCapsuleComponent();
	Cap->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	Cap->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
	PlayerHealth = StartingPlayerHealth;
	if (GetWorld()) {
		GameUI = CreateWidget<UInGameUI>(GetWorld(), MenuClass);
		GameUI->AddToViewport();
		GameUI->UpdateCurrentScore(CurrentScore);
		PlayerHealth = StartingPlayerHealth;

		
		GameUI->UpdateHealthBar(StartingPlayerHealth/PlayerHealth);
	}
	UActorComponent* MovementComp = this->GetComponentByClass(UCharacterMovementComponent::StaticClass());
	UCharacterMovementComponent* MoveCompCast = Cast<UCharacterMovementComponent, UActorComponent>(MovementComp);

	
	
}

void APlayerCharacter::WKey()
{
	if (PlayerOnGrid) {
		this->SetActorLocation(FVector(this->GetActorLocation().X + OnGridMoveDistance, this->GetActorLocation().Y, this->GetActorLocation().Z));
	}
	
}
void APlayerCharacter::SKey()
{
	if (PlayerOnGrid) {
	
		this->SetActorLocation(FVector(this->GetActorLocation().X - OnGridMoveDistance, this->GetActorLocation().Y, this->GetActorLocation().Z));
	}
	
}
void APlayerCharacter::DKey()
{
	if (PlayerOnGrid) {
		this->SetActorLocation(FVector(this->GetActorLocation().X , this->GetActorLocation().Y + OnGridMoveDistance, this->GetActorLocation().Z));
	}
	
}

void APlayerCharacter::AKey()
{
	if (PlayerOnGrid) {
		this->SetActorLocation(FVector(this->GetActorLocation().X, this->GetActorLocation().Y -	 OnGridMoveDistance, this->GetActorLocation().Z));
	}
	
}

void APlayerCharacter::SpaceBar()
{
	if (!ActionActor)
	{
		return;
	}
	if (ActionActor->IsA<AMemoryGridDeactivationConsole>())
	{
		if (Cast<AMemoryGridDeactivationConsole>(ActionActor)->StaticClass()->ImplementsInterface(UInteract::StaticClass()))
		{
			if (ActionActor && Cast<AMemoryGridDeactivationConsole>(ActionActor)->Active)
			{
				int NewScore = CurrentScore + Cast<AMemoryGridDeactivationConsole>(ActionActor)->ScoreToGive;

				GameUI->UpdateCurrentScore(NewScore);
				CurrentScore = NewScore;
				Cast<IInteract>(ActionActor)->Interact();
			}
		}
	}
	if (ActionActor->IsA<AMatch3>())
	{
		if (Cast<AMatch3>(ActionActor)->StaticClass()->ImplementsInterface(UInteract::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Show Console"));
			Cast<IInteract>(ActionActor)->Interact();
			Holdplayer = true;
			APlayerController* pc = GetWorld()->GetFirstPlayerController();

			pc->bShowMouseCursor = true;
			pc->bEnableClickEvents = true;
			pc->bEnableMouseOverEvents = true;

		}
		
	}



	
}
void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMemoryGrid>())
	{
		if (Cast<AMemoryGrid>(OtherActor)->status == GridStatus::Danger)
		{
			PlayerHealth = 1 / Cast<AMemoryGrid>(OtherActor)->DamageToDeal;	
	
			GameUI->UpdateHealthBar(PlayerHealth / StartingPlayerHealth);
			GameUI->UpdateHealthText(FString(FString::FromInt(PlayerHealth) + "/" + FString::FromInt(StartingPlayerHealth)));
			Cast<AAPortedStudyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->RespawnPlayer();
			PlayerOnGrid = false;
			return;
		}
		this->SetActorLocation(OtherActor->GetActorLocation());
		PlayerOnGrid = true;
	}

	if (OtherActor->IsA<AMemoryGridDeactivationConsole>())
	{
	
		MemoryGirdDeactivationFlag = true;
		ActionActor = OtherActor;
	}

	if (OtherActor->IsA<AMatch3>())
	{
		ConsoleTrigger = true;
		ActionActor = OtherActor;
	}
}
void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AMemoryGrid>())
	{
		PlayerOnGrid = false;
	}

	if (OtherActor->IsA<AMemoryGridDeactivationConsole>())
	{
		MemoryGirdDeactivationFlag = false;
		ActionActor = nullptr;		
	}
}


void APlayerCharacter::wsAxis(float val)
{
	FVector dir = FVector::ZeroVector;

	if (!PlayerOnGrid && !Holdplayer)
	{
		dir = this->GetActorForwardVector() * MoveSpeed * val;
		this->AddMovementInput(dir);	
		
	}

}

void APlayerCharacter::adAxis(float val)
{
	if (!PlayerOnGrid && !Holdplayer)
	{
		this->AddMovementInput(this->GetActorRightVector() * MoveSpeed * val);
		
		

	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	


}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Damage take"));
	return 0.0f;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (InputComponent)
	{	
		InputComponent->BindAxis("wsAxis", this, &APlayerCharacter::wsAxis);
		InputComponent->BindAxis("adAxis", this, &APlayerCharacter::adAxis);

		InputComponent->BindAction("WKey",EInputEvent::IE_Pressed,this, &APlayerCharacter::WKey);
		InputComponent->BindAction("SKey", EInputEvent::IE_Pressed, this, &APlayerCharacter::SKey);

		InputComponent->BindAction("AKey", EInputEvent::IE_Pressed, this, &APlayerCharacter::AKey);
		InputComponent->BindAction("DKey", EInputEvent::IE_Pressed, this, &APlayerCharacter::DKey);

		InputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &APlayerCharacter::SpaceBar);
	}

}

