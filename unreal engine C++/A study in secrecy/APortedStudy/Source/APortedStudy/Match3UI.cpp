// Fill out your copyright notice in the Description page of Project Settings.

#include "Match3UI.h"
#include "PlayerCharacter.h"
#include "Components/Button.h"

void UMatch3UI::NativeConstruct()
{
	CloseUI->OnClicked.AddDynamic(this, &UMatch3UI::CloseWindow);
	Super::NativeConstruct();
}

void UMatch3UI::CloseWindow()
{

	APlayerCharacter* pPawn = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	pPawn->Holdplayer = false;

	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->bShowMouseCursor = false;
	pc->bEnableClickEvents = false;
	pc->bEnableMouseOverEvents = false;

	this->RemoveFromViewport();
}