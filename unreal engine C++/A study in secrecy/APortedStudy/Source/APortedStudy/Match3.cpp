// Fill out your copyright notice in the Description page of Project Settings.


#include "Match3.h"
#include "Match3UI.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AMatch3::AMatch3()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));


	BoxCollider->SetBoxExtent(FVector(500, 100, 100));

	//BoxCollider->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	//BoxCollider->SetRelativeLocation(FVector(10, -300, 100));

	RootComponent = Mesh;
	Mesh->RegisterComponentWithWorld(GetWorld());
	UStaticMesh* meshModel = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Art/CUBE_-_Sci_Fi_Interiors_Pack/FBX/TitleScreenPC.TitleScreenPC")));
	Mesh->SetStaticMesh(meshModel);
	Mesh->SetWorldScale3D(FVector(0.15, 0.15, 0.15));

	ConstructorHelpers::FClassFinder<UMatch3UI> MenuClassFinder(TEXT("/Game/UI/MatchConsole"));
	Match3UIClass = MenuClassFinder.Class;

}

// Called when the game starts or when spawned
void AMatch3::BeginPlay()
{
	
	Super::BeginPlay();
	if (GetWorld()) {
		Match3UI = CreateWidget<UMatch3UI>(GetWorld(), Match3UIClass);
	}
}
void AMatch3::CloseWindow()
{
	Match3UI->RemoveFromViewport();
}
void AMatch3::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT(" Console"));
	Match3UI->AddToViewport();
	

}

// Called every frame
void AMatch3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

