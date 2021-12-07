// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PlayerUI.generated.h"


UCLASS()
class ISOROGUELIKE_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	void UpdateHealthBar(float NewVal);
	void UpdateEnergyBar(float NewVal);

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* EnergyBar;
};
