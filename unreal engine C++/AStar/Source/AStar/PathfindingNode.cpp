// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingNode.h"



PathfindingNode::PathfindingNode(bool _walkable, FVector _WorldPosition, int _GridLocation,int _GridX,int _GridY)
{
	Walkable = _walkable;
	WorldPosition = _WorldPosition;
	GridLocation = _GridLocation;
	// sort of redundant but useful for distance calculations
	GridX = _GridX;
	GridY = _GridY;
}

PathfindingNode::~PathfindingNode()
{
}
