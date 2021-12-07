// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();
}
void UPlayerUI::UpdateHealthBar(float NewVal)
{
	HealthBar->SetPercent(NewVal);
}
void UPlayerUI::UpdateEnergyBar(float NewVal)
{
	EnergyBar->SetPercent(NewVal);
}