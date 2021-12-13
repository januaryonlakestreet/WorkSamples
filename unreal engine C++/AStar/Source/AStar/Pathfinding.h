// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathfindingGrid.h"
#include "Pathfinding.generated.h"

UCLASS()
class ASTAR_API APathfinding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathfinding();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	APathfindingGrid* pfg;

	int GetDistance(PathfindingNode* A, PathfindingNode* B);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pathfinding")
		TArray<FVector> GeneratePath(FVector a, FVector b);
};
