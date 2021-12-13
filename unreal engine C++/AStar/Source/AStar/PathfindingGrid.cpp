// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingGrid.h"

#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
APathfindingGrid::APathfindingGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CreateDefaultSubobject<USceneComponent>(FName("Grid"));
}

// Called when the game starts or when spawned
void APathfindingGrid::BeginPlay()
{
	Super::BeginPlay();
	NodeDiameter = NodeRadius * 2;
	GridSizeX = FGenericPlatformMath::RoundToInt(GridWorldSize.X / NodeDiameter);
	GridSizeY = FGenericPlatformMath::RoundToInt(GridWorldSize.Y / NodeDiameter);
	
	
	FVector WorldBottomLeft = this->GetActorLocation() - FVector::RightVector * GridWorldSize.X / 2 - FVector::ForwardVector * GridWorldSize.Y / 2;
	for (int x = 0; x < GridSizeX; x++)
	{
		for (int y = 0; y < GridSizeY; y++)
		{
			FVector WorldPoint = WorldBottomLeft + FVector::RightVector * (x * NodeDiameter + NodeRadius) + FVector::ForwardVector * (y * NodeDiameter + NodeRadius)+10;
			bool Walkable = true;
			int Value = x + y;
			Grid.Add(new PathfindingNode(Walkable, WorldPoint,Value,x,y));
			WorldPositions.Add(WorldPoint);
		}
	}	
	
}

TArray<PathfindingNode*> APathfindingGrid::GetNeighbours(PathfindingNode* Node)
{
	TArray<PathfindingNode*> Neighbours;
	TArray<int> ValidValues;
	ValidValues.Add(Node->GridLocation + 1);
	ValidValues.Add(Node->GridLocation - 1);
	ValidValues.Add(Node->GridLocation + GridSizeY);
	ValidValues.Add(Node->GridLocation - GridSizeY);

	for (int a = 0; a < Grid.Num(); a++)
	{
		if (ValidValues.Contains(Grid[a]->GridLocation))
		{
			Neighbours.Add(Grid[a]);
		}
	}
	return Neighbours;
}

PathfindingNode* APathfindingGrid::NodeFromWorldPoint(FVector WorldPoint)
{
	float PercentX, PercentY;
	PercentX = (WorldPoint.X + GridWorldSize.X / 2) / GridWorldSize.X;
	PercentY = (WorldPoint.Z + GridWorldSize.Y / 2) / GridWorldSize.Y;

	PercentX = FMath::Clamp(PercentX, 0.0f, 1.0f);
	PercentY = FMath::Clamp(PercentY, 0.0f, 1.0f);

	int x = FMath::RoundToInt((GridSizeX - 1) * PercentX);
	int y = FMath::RoundToInt((GridSizeY - 1) * PercentY);

	int val = x + y;

	for (int a = 0; a < Grid.Num(); a++)
	{
		if (Grid[a]->GridLocation == val)
		{
			return Grid[a];
		}
	}
	return nullptr;
}




// Called every frame
void APathfindingGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

