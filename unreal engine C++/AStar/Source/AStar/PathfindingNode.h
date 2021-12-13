// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ASTAR_API PathfindingNode
{
public:
	PathfindingNode(bool _walkable, FVector _WorldPosition,int _GridLocation,int _GridX,int _GridY);
	~PathfindingNode();
	bool Walkable;
	PathfindingNode* Parent;
	FVector WorldPosition;
	int GridLocation;
	int GCost;
	int HCost;

	int GridX;
	int GridY;
	int FCost() { return GCost + HCost; }
};
