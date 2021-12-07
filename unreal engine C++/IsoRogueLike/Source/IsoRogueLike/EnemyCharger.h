// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MainAIController.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharger.generated.h"


UENUM()
enum ChargerStatus
{
	Idle UMETA(DisplayName = "Idle"),
	Moving UMETA(DisplayName = "Moving"),


};
UCLASS()
class ISOROGUELIKE_API AEnemyCharger : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	AMainAIController* ChargerController;
	AActor* PlayerCharacter;
	void ProcessStatus(ChargerStatus State);
	ChargerStatus IdleStateHandle();
	ChargerStatus MovingStateHandle();
	UPROPERTY(EditAnywhere);
	TEnumAsByte<ChargerStatus> Currentstatus;

	FTimerHandle ChargeResetTimerHandle;
	FTimerDelegate ChargeResetDelegate;
	UPROPERTY(BlueprintReadWrite);
	bool CanCharge{ true };
	UPROPERTY(BlueprintReadWrite);
	bool Charging{ false };
	UFUNCTION()
	void CanChargeReset();

	float ChargeLength{ 5.0 };
	float ChargeDistance{ 50.0 };
public:	
	UPROPERTY(BlueprintReadWrite);
	bool Moving;
	UPROPERTY(BlueprintReadWrite);
	FVector T;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
