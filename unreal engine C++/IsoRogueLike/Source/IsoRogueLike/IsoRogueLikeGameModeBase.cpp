// Copyright Epic Games, Inc. All Rights Reserved.


#include "IsoRogueLikeGameModeBase.h"
#include "PlayerCharacter.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Room.h"
#include "stdlib.h"
AIsoRogueLikeGameModeBase::AIsoRogueLikeGameModeBase()
{
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Blueprints/PlayerCharacter")));
	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	DefaultPawnClass = GeneratedBP->GeneratedClass;
	

	
}

void AIsoRogueLikeGameModeBase::BeginPlay()
{
	//step one - find the start room
	// step two - assign that's location to a variable
	//step three - offset that location by half room width
	//step four - check iff new location is not in a list 
	//step five - if it is not in the list add a room
	//step six - go to step two assigning the new rooms location to the variable.
	TArray<ARoom*> RoomList;
	TArray<AActor*> StartRoomRefs;
	AActor* StartRoom;
	FVector RoomLocation = FVector::ZeroVector;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoom::StaticClass(), StartRoomRefs);
	if (StartRoomRefs.Num() == 1)
	{
		StartRoom = StartRoomRefs[0];
		//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, FString("start room found"));
		UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(StartRoom->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		FVector offset = StaticMeshComp->GetStaticMesh()->GetBounds().BoxExtent * 60;
		float Scale = StartRoom->GetActorScale().X;
		offset = offset * Scale;
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, offset.ToString() + " " + offset.ToString());
		
		RoomList.Add(Cast<ARoom>(StartRoom));
		for (int a = 0; a < MaxNumberOfRooms; a++)
		{	
			int DirectionValue = PickDirection();
			GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red,FString::FromInt(DirectionValue));
			//spawn new room here

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
			SpawnParameters.bNoFail = true;


			UClass* GeneratedClass = StartRoom->GetClass();

			AActor* NewRoom = GetWorld()->SpawnActor(GeneratedClass);
			NewRoom->SetActorLocation(StartRoom->GetActorLocation());
			FVector DirectionOffset = FVector::ZeroVector;
			switch (DirectionValue)
			{
			case 1:
				DirectionOffset = FVector(offset.X, 0, 0);
			break;
			case 2:
				DirectionOffset = FVector(-offset.X, 0, 0);
				break;
			case 3:
				DirectionOffset = FVector(0, offset.X, 0);
				break;
			case 4:
				DirectionOffset = FVector(0, -offset.X, 0);
				break;
			}
			NewRoom->SetActorLocation(NewRoom->GetActorLocation() + DirectionOffset);
			StartRoom = NewRoom;
			RoomList.Add(Cast<ARoom>(NewRoom));			
		}
		for (int a = 0; a < RoomList.Num(); a++)
		{
			RoomList[a]->PlaceWalls();
		}

	}
}
int AIsoRogueLikeGameModeBase::PickDirection()
{	
	return rand() % 4 + 1;
}