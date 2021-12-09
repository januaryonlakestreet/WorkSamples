// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MemoryGridManager.h"
#include "Components/StaticMeshComponent.h"
#include "Interact.h"
#include "MemoryGridDeactivationConsole.generated.h"

UCLASS()
class APORTEDSTUDY_API AMemoryGridDeactivationConsole : public AActor , public IInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMemoryGridDeactivationConsole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact() override;
	UPROPERTY(EditAnywhere);
	int	MemoryGridID;
	bool Active = true;

	int ScoreToGive{ 10 };
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollider;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

};
