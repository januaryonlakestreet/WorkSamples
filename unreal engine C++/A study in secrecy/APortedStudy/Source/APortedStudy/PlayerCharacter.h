// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGameUI.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class APORTEDSTUDY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void wsAxis(float val);
	void adAxis(float val);

	void WKey();
	void SKey();
	void DKey();
	void AKey();

	void SpaceBar();
	

	bool MemoryGirdDeactivationFlag{ false };

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	AActor* ActionActor;
	TSubclassOf<class UUserWidget> MenuClass;
	bool PlayerOnGrid{ false };
	bool ConsoleTrigger{ false };
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);
	int CurrentScore{ 0 };
	UPROPERTY(EditAnywhere)
		 float OnGridMoveDistance{ 200 };
	UPROPERTY(EditAnywhere)
		float MoveSpeed{ 4 };
	UInGameUI* GameUI;
	float PlayerHealth;
	float StartingPlayerHealth{ 10 };
	bool Holdplayer = false;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
