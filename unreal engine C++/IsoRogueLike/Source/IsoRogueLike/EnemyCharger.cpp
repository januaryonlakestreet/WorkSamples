// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharger.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AEnemyCharger::AEnemyCharger()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyCharger::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Player;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), Player);
	if (Player.Num() > 0)
	{
		PlayerCharacter = Player[0];
	}
	ChargerController = Cast<AMainAIController>(GetController());
	Currentstatus = ChargerStatus::Idle;
	ChargeResetDelegate.BindUFunction(this, FName("CanChargeReset"));
}

void AEnemyCharger::ProcessStatus(ChargerStatus State)
{
	switch (State)
	{
	case ChargerStatus::Idle:
		Currentstatus = IdleStateHandle();
		break;
	case ChargerStatus::Moving:
		Currentstatus = MovingStateHandle();
		break;
	default:
		break;
	}
}

ChargerStatus AEnemyCharger::IdleStateHandle()
{
	if (PlayerCharacter)
	{
		return ChargerStatus::Moving;
	}
	return ChargerStatus::Idle;
}

ChargerStatus AEnemyCharger::MovingStateHandle()
{
	if (Charging)
	{
		T = this->GetActorLocation();
		FVector Location = this->GetActorForwardVector() + 100;
		ChargerController->MoveToLocation(Location);
	}
	else
	{
		ChargerController->MoveToLocation(PlayerCharacter->GetActorLocation());
	}
	
	return ChargerStatus::Moving;
}

// Called every frame
void AEnemyCharger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Currentstatus != ChargerStatus::Moving)
	{
		Moving = false;
	}
	else
	{
		Moving = true;
	}
	ProcessStatus(Currentstatus);



	if (FVector::Dist(this->GetActorLocation(), PlayerCharacter->GetActorLocation()) < 550 && CanCharge)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString("CHARGING"));
		Charging = true;
		CanCharge = false;
		//GetWorld()->GetTimerManager().SetTimer(ChargeResetTimerHandle, ChargeResetTimerDelegate, 0.5, false, 5);
	}
	
}
void AEnemyCharger::CanChargeReset()
{
	Charging = false;
	CanCharge = true;
}
// Called to bind functionality to input
void AEnemyCharger::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


