// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyWaypoint.h"
#include "Kismet\GameplayStatics.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnController = Cast<AEnemyController>(GetController());
	
	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	TArray<AActor*> Out;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), Out);
	if (Out.Num() > 0)
	{
		Player = Cast<APlayerCharacter,AActor>(Out[0]);
	}
}

void AEnemy::ProcessState(States InState)
{
	switch (InState)
	{
	case AEnemy::States::Patrolling:
		CurrentState = PatrolHandle();
		break;
	case AEnemy::States::Idle:
		CurrentState = IdleHandle();
		break;
	case AEnemy::States::ChasingPlayer:
		CurrentState = ChasingPlayerHandle();
	case AEnemy::States::AttackingPlayer:
		CurrentState = AttackingPlayerHandle();
	}
}
void AEnemy::NextGoal()
{
	if (PatrolPointID + 1 < PatrolPoints.Num())
	{
		PatrolPointID++;
		Goal = ReturnPatrolPointById(PatrolPointID);
		return;
	}
	PatrolPointID = 0;
	Goal = ReturnPatrolPointById(PatrolPointID);
}
AEnemy::States AEnemy::ChasingPlayerHandle()
{
	EnController->MoveToLocation(Player->GetActorLocation());
	if (FVector::Dist(this->GetActorLocation(), Player->GetActorLocation()) < AttackDistance)
	{
	
		EnController->StopMovement();
		return States::AttackingPlayer;
	}
	return States::ChasingPlayer;
}
AEnemy::States AEnemy::AttackingPlayerHandle()
{
	if (FVector::Dist(this->GetActorLocation(), Player->GetActorLocation()) > AttackDistance)
	{
		return States::ChasingPlayer;
	}
	EnController->StopMovement();
	if (CanFire == true)
	{
		FDamageEvent dEvent;
	
		Player->TakeDamage(10,dEvent, EnController,this);
		CanFire = false;
		GetWorld()->GetTimerManager().SetTimer(ShootResetHandle,this,&AEnemy::ResetCanFire,FireRate,false);

	}
	return States::AttackingPlayer;
}
AEnemy::States AEnemy::IdleHandle()
{
	NextGoal();
	if (Goal) {
		return AEnemy::States::Patrolling;
	}
	return States::Idle;
}
AEnemy::States AEnemy::PatrolHandle()
{
	EnController->MoveToLocation(Goal->GetActorLocation());
	Dist = FVector::Distance(Goal->GetActorLocation(), this->GetActorLocation());
	if (FVector::Distance(Goal->GetActorLocation(), this->GetActorLocation()) < 100)
	{
		NextGoal();
		return AEnemy::States::Patrolling;
	}
	return States::Patrolling;
}
AEnemyWaypoint* AEnemy::ReturnPatrolPointById(int id)
{
	for (AEnemyWaypoint* Waypointitr : PatrolPoints)
	{
		if (Waypointitr->ID == id)
		{
			return Waypointitr;
		}
	}
	return PatrolPoints[0];
}
void AEnemy::ResetCanFire()
{
	CanFire = true;
}
// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProcessState(CurrentState);

	if (EnController->LineOfSightTo(Player) && FVector::Dist(this->GetActorLocation(),Player->GetActorLocation()) < SightDistance)
	{

		CurrentState = AEnemy::States::ChasingPlayer;
	}

	if (EnController->LineOfSightTo(Player) && FVector::Dist(this->GetActorLocation(), Player->GetActorLocation()) > SightDistance &&
		CurrentState == AEnemy::States::ChasingPlayer || CurrentState == AEnemy::States::AttackingPlayer)
	{
	
	
		CurrentState = AEnemy::States::Idle;
	}


	
}
bool AEnemy::CanISeePlayer() {
	return false;
}
// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

