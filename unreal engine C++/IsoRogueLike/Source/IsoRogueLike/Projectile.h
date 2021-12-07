// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Projectile.generated.h"

UCLASS()
class ISOROGUELIKE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Your Category", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjMovement;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UStaticMeshComponent* ProjectileMeshComponent;
	void FireInDirection(const FVector& ShootDirection,AActor* _SpawnedBy);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	AActor* SpawnedBy;
	FVector Direction;
	float ProjectileLife{ 30.f };
	FTimerHandle ProjectileLifeTimerHandle;
	FTimerDelegate ProjectileLifeDel;
	UFUNCTION()
		void ProjectileDie();

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
