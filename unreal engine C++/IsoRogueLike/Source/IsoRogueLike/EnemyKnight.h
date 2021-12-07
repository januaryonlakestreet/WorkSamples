// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "MainAIController.h"
#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "Projectile.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnemyHealthBar.h"
#include "EnemyKnight.generated.h"


UENUM()
enum KnightStatus
{
	Idle UMETA(DisplayName = "Idle"),
	Moving UMETA(DisplayName = "Moving"),
	
};

UCLASS()
class ISOROGUELIKE_API AEnemyKnight : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyKnight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	AActor* PlayerActor;
	float dist;
	void ProcessStatus(KnightStatus State);
	KnightStatus IdleStateHandle();
	KnightStatus MovingStateHandle();
	
	FTimerHandle CanFireTimerHandle;
	FTimerDelegate CanFireTimerDelegate;

	FTimerHandle ActuallyFireTimerHandle;
	FTimerDelegate ActuallyFireTimerDelegate;
	UFUNCTION()
		void Fire();


	UEnemyHealthBar* HealthBar;
	UWidgetComponent* EnemyHealthWidget;
	FVector StartLocation;
	AMainAIController* KnightController;

	UCapsuleComponent* CollisionComponent;
	void SpawnProjectile();
	bool CanFire{ true };
	AProjectile* Projectile;
	float GetRotationSpeed();
	UFUNCTION()
	void ResetCanFire();
	float Mass;
	FVector Impact;
	void AddImpact(FVector Direction, float Force);
public:	
	// Called every frame
	UPROPERTY(BlueprintReadWrite);
	bool Moving;

	virtual void Tick(float DeltaTime) override;
	USceneComponent* ShotSpawn;
	UPROPERTY(EditAnywhere);
	float RotationSpeedNormal{ 2.5 };
	UPROPERTY(EditAnywhere);
	float RotationSpeedProjectile{ 1.5 };
	UPROPERTY(EditAnywhere);
	float AggroRange{ 500 };	
	UPROPERTY(EditAnywhere);
	float CombatRange{ 400 };
	UPROPERTY(EditAnywhere);
	TEnumAsByte<KnightStatus> Currentstatus;
	
	
	UPROPERTY(EditAnywhere);
	float KnightHealthStart{ 10.0 };
	UPROPERTY(EditAnywhere);
	float KnightHealthCurrent;
	UPROPERTY(EditAnywhere);
	float DelayBetweenShowingAndFiring{ 2.0 };
	UPROPERTY(EditAnywhere);
	float DelayBetweenShowingNewShots{ 6.0 };
	
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

