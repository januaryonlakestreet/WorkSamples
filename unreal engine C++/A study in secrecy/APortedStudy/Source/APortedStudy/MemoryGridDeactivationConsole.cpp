// Fill out your copyright notice in the Description page of Project Settings.


#include "MemoryGridDeactivationConsole.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MemoryGridManager.h"
#include "Engine/World.h"

// Sets default values
AMemoryGridDeactivationConsole::AMemoryGridDeactivationConsole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));


	BoxCollider->SetBoxExtent(FVector(250, 100, 100));

	//BoxCollider->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	BoxCollider->SetRelativeLocation(FVector(0, 0, 0));

	RootComponent = Mesh;	
	Mesh->RegisterComponentWithWorld(GetWorld());
	UStaticMesh* meshModel = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Game/Art/CUBE_-_Sci_Fi_Interiors_Pack/FBX/TitleScreenPC.TitleScreenPC")));
	Mesh->SetStaticMesh(meshModel);
	Mesh->SetWorldScale3D(FVector(0.15, 0.15, 0.15));
	
}
void AMemoryGridDeactivationConsole::Interact()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMemoryGridManager::StaticClass(), OutActors);

	for (AActor* ActorItr : OutActors)
	{
		if (Cast<AMemoryGridManager>(ActorItr)->GridId == MemoryGridID)
		{
			Cast<AMemoryGridManager>(ActorItr)->Deactivate();
			Active = false;
		}
	}
}

// Called when the game starts or when spawned
void AMemoryGridDeactivationConsole::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMemoryGridDeactivationConsole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

