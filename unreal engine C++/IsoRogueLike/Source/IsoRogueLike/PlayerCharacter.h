// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerUI.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ISOROGUELIKE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Forward(float val);
	void Sideways(float val);
	
	void Fire();
	FVector GetLookDirection();

	UCapsuleComponent* CollisionComponent;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere);
	float ShotEnergyCost;
	UPROPERTY(EditAnywhere);
	float EnergyRechargeRate;
	UPROPERTY(EditAnywhere);
	float CurrentEnergy;
	UPROPERTY(EditAnywhere);
	float MaxEnergy;
	UPROPERTY(EditAnywhere);
	float MoveSpeed{ 10 };
	
	UPROPERTY(EditAnywhere);
	float PlayerHealthCurrent;
	UPROPERTY(EditAnywhere);
	float PlayerHealthMax{ 10 };

	UPROPERTY(EditAnywhere);
	USceneComponent* ShotSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool Moving;
	
	

	UPROPERTY(EditAnywhere);
	bool CanFire{ false };

	UPROPERTY(EditAnywhere);
	float CanFireDelay{ 0.1 };

	TSubclassOf<class UUserWidget> MenuClass;
	UPlayerUI* PlayerHud;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	FTimerHandle CanFireTimerHandle;
	FTimerDelegate CanFireTimerDelegate;
	float Mass;
	FVector Impact;
	void AddImpact(FVector Direction, float Force);

	UFUNCTION()
	void CanFireReset();
};
