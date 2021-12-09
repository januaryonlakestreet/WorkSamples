// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "PlayerCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "APortedStudyGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class APORTEDSTUDY_API AAPortedStudyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AAPortedStudyGameModeBase(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void RespawnPlayer();
protected:
	FVector PlayerSpawnPoint;
	APlayerCharacter* PlayerCharacter;
	float LevelTimer{ 0.0 };
	float LevelTimeStep{ 0.001 };
};
