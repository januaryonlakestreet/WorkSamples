// Copyright Epic Games, Inc. All Rights Reserved.


#include "APortedStudyGameModeBase.h"
#include "PlayerCharacter.h"
#include "UObject/UObjectGlobals.h"
#include "GameFramework/PlayerStart.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
AAPortedStudyGameModeBase::AAPortedStudyGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/BP/MainPlayer")));
	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = GeneratedBP->GeneratedClass;
}

void AAPortedStudyGameModeBase::BeginPlay()
{	
	TArray<AActor*> PlayerSpawnActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerSpawnActor);
	PlayerSpawnPoint = PlayerSpawnActor[0]->GetActorLocation();

	if (PlayerSpawnActor.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("No spawn point")));
	}

	TArray<AActor*> PossiblePlayers; 
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), PossiblePlayers);
	PlayerCharacter = Cast<APlayerCharacter>(PossiblePlayers[0]);

	TArray<AActor*> Camera;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Camera);
	GetWorld()->GetFirstPlayerController()->SetViewTarget(Camera[0]);
}

void AAPortedStudyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LevelTimer += LevelTimeStep;
	PlayerCharacter->GameUI->UpdateCurrentTime(LevelTimer);
	

}

void AAPortedStudyGameModeBase::RespawnPlayer()
{
	PlayerCharacter->SetActorLocation(PlayerSpawnPoint);
}