// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

/**
 * 
 */
UCLASS()
class APORTEDSTUDY_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()


public:

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentScore;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* PlayerHealthBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerHealthText;

	void UpdateCurrentTime(float NewTime);
	void UpdateCurrentScore(int NewScore);
	void UpdateHealthBar(float NewScale);
	void UpdateHealthText(FString NewString);

private:

};
