// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MemoryGridManager.generated.h"

UCLASS()
class APORTEDSTUDY_API AMemoryGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMemoryGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	int _MovesTillChange;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
		bool PlayerOnGrid;

	UPROPERTY(EditAnywhere);
	int GridId;

	UPROPERTY(EditAnywhere);
	int MovesTillChange;

	UPROPERTY(EditAnywhere);
	TArray<AActor*> Grid;
	void PlayerEntersGrid();
	void PlayerLeavesGrid();
	void PlayerTakeStepOnGrid();
	bool IsPlayerOnGrid() { return PlayerOnGrid; }

	void Deactivate();
};
