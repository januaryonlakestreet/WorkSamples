// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWaypoint.h"
#include "Components/SceneComponent.h"

// Sets default values
AEnemyWaypoint::AEnemyWaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
}

// Called when the game starts or when spawned
void AEnemyWaypoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

