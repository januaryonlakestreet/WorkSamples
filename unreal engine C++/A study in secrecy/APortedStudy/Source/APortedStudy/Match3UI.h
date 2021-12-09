// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Match3UI.generated.h"

/**
 * 
 */
UCLASS()
class APORTEDSTUDY_API UMatch3UI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		class UButton* CloseUI;
	virtual void NativeConstruct() override;

	UFUNCTION()
		void CloseWindow();
};
