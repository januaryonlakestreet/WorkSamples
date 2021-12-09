// Fill out your copyright notice in the Description page of Project Settings.


#include "MemoryGridManager.h"
#include "MemoryGrid.h"
// Sets default values
AMemoryGridManager::AMemoryGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	
}

// Called when the game starts or when spawned
void AMemoryGridManager::BeginPlay()
{
	Super::BeginPlay();
	MovesTillChange = 3;
	_MovesTillChange = MovesTillChange;
}

// Called every frame
void AMemoryGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMemoryGridManager::PlayerEntersGrid()
{
	
	PlayerOnGrid = true;
}

void AMemoryGridManager::PlayerLeavesGrid()
{

	PlayerOnGrid = false;
}

void AMemoryGridManager::PlayerTakeStepOnGrid()
{
	

	if (MovesTillChange  == 0)
	{
		MovesTillChange = _MovesTillChange;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Change state"));
		for (AActor* ActorItr : Grid)
		{
			AMemoryGrid* GridSquare = Cast<AMemoryGrid>(ActorItr);
			GridSquare->ChangeStatus();
		
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("minus"));
		MovesTillChange--;
	}
}

void AMemoryGridManager::Deactivate()
{
	for (AActor* ActorItr : Grid)
	{
		Cast<AMemoryGrid>(ActorItr)->Deactivate();
	}
}

