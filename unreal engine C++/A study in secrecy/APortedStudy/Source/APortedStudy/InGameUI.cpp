// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"


void UInGameUI::UpdateHealthBar(float NewScale)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Black, FString::SanitizeFloat(NewScale));
	PlayerHealthBar->SetPercent(NewScale);
}

void UInGameUI::UpdateHealthText(FString NewString)
{
}


void UInGameUI::UpdateCurrentTime(float NewTime)
{
	CurrentTime->SetText(FText::FromString(FString::SanitizeFloat(NewTime)));
}
void UInGameUI::UpdateCurrentScore(int NewScore)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), OutActors);
	CurrentScore->SetText(FText::FromString(FString::FromInt(NewScore)));
	
}