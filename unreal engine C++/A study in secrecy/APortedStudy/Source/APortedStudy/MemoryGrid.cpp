// Fill out your copyright notice in the Description page of Project Settings.


#include "MemoryGrid.h"
#include "Components/BoxComponent.h"
#include "APortedStudyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AMemoryGrid::AMemoryGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	box->RegisterComponentWithWorld(GetWorld());
	RootComponent = Scene;

	box->SetBoxExtent(FVector(50, 50, 25));
	
	//box->AttachToComponent(RootComponent,FAttachmentTransformRules::SnapToTargetIncludingScale);


	static ConstructorHelpers::FObjectFinder<UMaterial> safemat(TEXT("/Game/Materials/SafeMaterial.SafeMaterial"));
	static ConstructorHelpers::FObjectFinder<UMaterial> WarningMat(TEXT("/Game/Materials/WarningMaterial.WarningMaterial"));
	static ConstructorHelpers::FObjectFinder<UMaterial> DangerMat(TEXT("/Game/Materials/DangerMaterial.DangerMaterial"));

	if (safemat.Object)
	{
		SafeMaterial = Cast<UMaterial>(safemat.Object);
	}
	if (WarningMat.Object)
	{
		WarningMaterial = Cast<UMaterial>(WarningMat.Object);
	}
	if (DangerMat.Object)
	{
		DangerMaterial = Cast<UMaterial>(DangerMat.Object);
	}
	
}

// Called when the game starts or when spawned
void AMemoryGrid::BeginPlay()
{
	Super::BeginPlay();

	box->OnComponentBeginOverlap.AddDynamic(this,&AMemoryGrid::OnOverlapBegin);
	box->OnComponentEndOverlap.AddDynamic(this, &AMemoryGrid::OnOverlapEnd);
	

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMemoryGridManager::StaticClass(), OutActors);
	
	for (AActor* MemManagerItr : OutActors)
	{
		if (Cast<AMemoryGridManager>(MemManagerItr)->GridId == ManagerID)
		{
			Manager = Cast<AMemoryGridManager>(MemManagerItr);
			Manager->Grid.Add(this);
		}
	}
	if (!Manager)
	{
		GetWorld()->DestroyActor(this);
	}
}
void AMemoryGrid::Deactivate()
{
	status = GridStatus::Deactivated;
}
void AMemoryGrid::ChangeStatus()
{
	TArray<AActor*> overlappingActors;
	switch (status)
	{
	case GridStatus::Safe:
		status = GridStatus::Warning;
		break;
	case GridStatus::Warning:
		status = GridStatus::Danger;	
		box->GetOverlappingActors(overlappingActors);
		
		for (AActor* ActorItr : overlappingActors)
		{
			if (ActorItr->IsA(APlayerCharacter::StaticClass()))
			{
				Cast<AAPortedStudyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->RespawnPlayer();
			
			}
		}

		break;
	case GridStatus::Danger:
		status = GridStatus::Safe;
		break;
	case GridStatus::Deactivated:
		break;
	default:
		break;
	}
}

void AMemoryGrid::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<APlayerCharacter>() && !Checked)
	{
		Manager->PlayerTakeStepOnGrid();
		FString OAName = OverlappedComp->GetName();
		

		
		Checked = true;
	}
}

void AMemoryGrid::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<APlayerCharacter>())
	{
		Manager->PlayerLeavesGrid();	
		Checked = false;
		
	}
}

// Called every frame
void AMemoryGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

