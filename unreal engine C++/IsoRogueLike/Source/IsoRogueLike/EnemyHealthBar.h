// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class ISOROGUELIKE_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()
public :
	virtual void NativeConstruct() override;
	void UpdateHealthbar(float NewVal);

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Healthbar;

	AActor* AttachedTo;
};
