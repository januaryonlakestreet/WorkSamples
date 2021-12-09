// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Match3UI.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Interact.h"
#include "Match3.generated.h"

UCLASS()
class APORTEDSTUDY_API AMatch3 : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMatch3();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TSubclassOf<class UUserWidget> Match3UIClass;
	UMatch3UI *Match3UI;
	
public:	
	void Interact() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UBoxComponent* BoxCollider;
	UFUNCTION()
	void CloseWindow();
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;
};
