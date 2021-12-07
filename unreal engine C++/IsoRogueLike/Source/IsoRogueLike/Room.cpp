// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Blueprints/ReplacementWall")));
	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	Wall = GeneratedBP->GeneratedClass;

	UObject* EnemyRangedActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Blueprints/EnemyKnightSwordBP")));
	UBlueprint* EnemyRangedBP = Cast<UBlueprint>(EnemyRangedActor);
	EnemyRanged = EnemyRangedBP->GeneratedClass;
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();	
}


// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoom::SpawnEnemies()
{
	TArray<UActorComponent*> EnemySpawnLocations;
	EnemySpawnLocations = GetComponentsByClass(USceneComponent::StaticClass());
	for (int a = 0; a < EnemySpawnLocations.Num(); a++)
	{
		if (EnemySpawnLocations[a]->ComponentHasTag(FName("EnemySpawn")))
		{
			float val = ((double)rand() / (RAND_MAX));
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, FString::SanitizeFloat(val));
			if (val < 0.5f)
			{
				FActorSpawnParameters params;
				FVector location = Cast<USceneComponent>(EnemySpawnLocations[a])->GetComponentLocation();
				location -= FVector(0, 0, 53);
				FRotator rotation = Cast<USceneComponent>(EnemySpawnLocations[a])->GetComponentRotation();
				GetWorld()->SpawnActor<AActor>(EnemyRanged, location, rotation, params);
			}
		}
	}
	
}


void ARoom::PlaceWalls()
{
	//get all box colliders
	//box cast get all using collider transfrom
	//if result contains less than 2 rooms spawn a wall.
	TArray<UActorComponent*> PossibleWallExits;
	PossibleWallExits = GetComponentsByClass(UBoxComponent::StaticClass());



	TArray<AActor*> OverlappingActors;
	for (int a = 0; a < PossibleWallExits.Num(); a++)
	{
		if (PossibleWallExits[a]->ComponentHasTag(FName("WallCheck")))
		{
			Cast<UBoxComponent>(PossibleWallExits[a])->GetOverlappingActors(OverlappingActors);
				int counter = 0;
				for (int b = 0; b < OverlappingActors.Num(); b++)
				{
					if (OverlappingActors[b]->IsA<ARoom>())
					{
						counter++;
					}
				}
			if (counter <= 2)
			{
				if (GetWorld())
				{
					FActorSpawnParameters params;
					FVector location = Cast<USceneComponent>(PossibleWallExits[a])->GetComponentLocation();
					location -= FVector(0, 0, 53);
					FRotator rotation = Cast<USceneComponent>(PossibleWallExits[a])->GetComponentRotation();
					GetWorld()->SpawnActor<AActor>(Wall, location, rotation, params);
					
				}
			}
			PossibleWallExits[a]->DestroyComponent();
		}
		
	}
	SpawnEnemies();
}

