// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IsoRogueLikeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ISOROGUELIKE_API AIsoRogueLikeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	void BeginPlay() override;
	AIsoRogueLikeGameModeBase();

	int MaxNumberOfRooms{ 8 };
	int PickDirection();
};
