// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCamera.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

#include "PlayerCharacter.h"
// Sets default values
AGameCamera::AGameCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	//Cast<UCameraComponent>(RootComponent)->SetProjectionMode(ECameraProjectionMode::Orthographic);
}

// Called when the game starts or when spawned
void AGameCamera::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> OutActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), OutActor);
	if (OutActor.Num() > 0)
	{
		Target = OutActor[0];
		OffSet = this->GetActorLocation() - Target->GetActorLocation();
	}
}

// Called every frame
void AGameCamera::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

	if (Target)
	{
		this->SetActorLocation(Target->GetActorLocation() + OffSet);		
		this->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Target->GetActorLocation()));
	}

}

