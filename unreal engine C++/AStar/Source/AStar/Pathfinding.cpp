// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfinding.h"
#include "PathfindingNode.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
APathfinding::APathfinding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathfinding::BeginPlay()
{
	Super::BeginPlay();
	
}
// Called every frame
void APathfinding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
TArray<FVector> APathfinding::GeneratePath(FVector a, FVector b)
{
	TArray<AActor*> out;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APathfindingGrid::StaticClass(), out);
	pfg = Cast<APathfindingGrid>(out[0]);
	


	if (!pfg)
	{
		return TArray<FVector>();
	}
	TArray<FVector> Path;

	PathfindingNode* StartNode;
	PathfindingNode* EndNode;
	
	StartNode = pfg->NodeFromWorldPoint(a);
	EndNode = pfg->NodeFromWorldPoint(b);

	TArray<PathfindingNode*> OpenSet;
	TArray<PathfindingNode*> ClosedSet;
	OpenSet.Add(StartNode);

	while (OpenSet.Num() > 0)
	{
		PathfindingNode* Node = OpenSet[0];
		for (int a_ = 0; a_ < OpenSet.Num(); a_++)
		{
			if (OpenSet[a_]->FCost() < Node->FCost() || OpenSet[a_]->FCost() == Node->FCost())
			{
				if (OpenSet[a_]->HCost < Node->HCost)
				{
					Node = OpenSet[a_];
				}
			}
		}
		OpenSet.Remove(Node);
		ClosedSet.Add(Node);

		if (Node == EndNode)
		{
			for (int val = 0; val < ClosedSet.Num(); val++)
			{
				Path.Add(ClosedSet[val]->WorldPosition);
			}
			return Path;
		}

		TArray<PathfindingNode*> NeighbourNodes = pfg->GetNeighbours(Node);
		for (int x = 0; x < NeighbourNodes.Num(); x++)
		{
			if (ClosedSet.Contains(NeighbourNodes[x])) { continue; }

			int NewCostToNeighbour = Node->GCost + GetDistance(Node, NeighbourNodes[x]);
			if (NewCostToNeighbour < NeighbourNodes[x]->GCost || !OpenSet.Contains(NeighbourNodes[x]))
			{
				NeighbourNodes[x]->GCost = NewCostToNeighbour;
				NeighbourNodes[x]->HCost = GetDistance(NeighbourNodes[x], EndNode);
				NeighbourNodes[x]->Parent = Node;
				if (!OpenSet.Contains(NeighbourNodes[x]))
				{
					OpenSet.Add(NeighbourNodes[x]);
				}
			}
		}
		
	}
	return Path;
}
int APathfinding::GetDistance(PathfindingNode* A, PathfindingNode* B)
{
	int distX = FMath::Abs(A->GridX - B->GridX);
	int distY = FMath::Abs(B->GridY - B->GridY);
	if (distX > distY)
	{
		return 14 * distY + 10 * (distX - distY);
	}
	return 14 * distX + 10 * (distY-distX);
}

