// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathfindingNode.h"
#include "GameFramework/Actor.h"
#include "PathfindingGrid.generated.h"




UCLASS()
class ASTAR_API APathfindingGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathfindingGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	float NodeDiameter;
	int GridSizeX, GridSizeY;


	TArray<PathfindingNode*>Grid;

	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FVector> WorldPositions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector2D GridWorldSize;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float NodeRadius;

	PathfindingNode* NodeFromWorldPoint(FVector WorldPoint);
	TArray<PathfindingNode*> GetNeighbours(PathfindingNode* Node);
};
