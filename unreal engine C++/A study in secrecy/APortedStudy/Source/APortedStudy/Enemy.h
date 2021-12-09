// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyController.h"
#include "PlayerCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyWaypoint.h"
#include "Enemy.generated.h"

UCLASS()
class APORTEDSTUDY_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	enum States {
		Idle, Patrolling, ChasingPlayer,AttackingPlayer
	};
	bool CanISeePlayer();
	States CurrentState = States::Idle;
	void ProcessState(States CurrentState);
	States PatrolHandle();
	States IdleHandle();
	States ChasingPlayerHandle();
	States AttackingPlayerHandle();
	APlayerCharacter* Player;
	AEnemyController* EnController;
	FVector DeparturePoint;
	AEnemyWaypoint* ReturnPatrolPointById(int id);
	bool CanFire{ true };
	UFUNCTION()
	void ResetCanFire();

	FTimerDelegate ShootResetDel;
	FTimerHandle ShootResetHandle;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void NextGoal();
	UPROPERTY(EditAnywhere)
		TArray<AEnemyWaypoint*> PatrolPoints;
	UPROPERTY(EditAnywhere)
		int PatrolPointID{ 0 };
	UPROPERTY(EditAnywhere)
		float Dist{ 0 };

	UPROPERTY(EditAnywhere)
		float AttackDistance{ 700 };
	UPROPERTY(EditAnywhere)
		float SightDistance{ 1000 };

	UPROPERTY(EditAnywhere)
	AActor* Goal;

	UPROPERTY(EditAnywhere)
		float FireRate{ 3 };


	UPROPERTY(EditAnywhere)
		float Angle;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
