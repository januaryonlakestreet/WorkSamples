// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MemoryGridManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Materials/Material.h"

#include "MemoryGrid.generated.h"


UENUM(BlueprintType)
enum GridStatus
{
	Safe UMETA(DisplayName = "Safe"),
	Warning	UMETA(DisplayName = "Warning"),
	Danger  UMETA(DisplayName = "Danger"),
	Deactivated UMETA(DisplayName = "Deactivated"),
};


UCLASS()
class APORTEDSTUDY_API AMemoryGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMemoryGrid();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	AMemoryGridManager* Manager;
	UBoxComponent* box;
	bool Checked{ false };
	

	
public:	
	// Called every frame
	void ChangeStatus();
	void Deactivate();

	float DamageToDeal{ 10 };

	UPROPERTY( BlueprintReadWrite)
		UMaterial* SafeMaterial;
	UPROPERTY(BlueprintReadWrite)
		UMaterial* WarningMaterial;
	UPROPERTY(BlueprintReadWrite)
		UMaterial* DangerMaterial;

	UFUNCTION(BlueprintCallable)
		TEnumAsByte<GridStatus> GetGridStatus() { return status; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		TEnumAsByte<GridStatus> status;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
		int ManagerID;


};
