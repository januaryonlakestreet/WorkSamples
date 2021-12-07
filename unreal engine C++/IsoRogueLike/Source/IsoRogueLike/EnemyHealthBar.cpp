// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthBar.h"
#include "Components/ProgressBar.h"
void UEnemyHealthBar::UpdateHealthbar(float NewVal)
{
	if (Healthbar)
	{
		Healthbar->SetPercent(NewVal);
	}
	
}

void UEnemyHealthBar::NativeConstruct()
{
	
	Super::NativeConstruct();
	if (!Healthbar) {
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, FString("Hea"));
	}
	
}