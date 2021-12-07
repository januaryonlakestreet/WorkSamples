// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"

#include "EnemyKnight.h"
// Sets default values
AProjectile::AProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    } 
    if (!ProjectileMeshComponent)
    {
        ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
        static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
        if (Mesh.Succeeded())
        {
            ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
            ProjectileMeshComponent->SetupAttachment(RootComponent);
        }
    }
    if (!CollisionComponent)
    {
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
        CollisionComponent->SetupAttachment(RootComponent);
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
       
    }
  
}

void AProjectile::FireInDirection(const FVector& ShootDirection,AActor* _SpawnedBy)
{
  //  ProjMovement->Velocity = ShootDirection * ProjMovement->InitialSpeed;
    this->Direction = ShootDirection;
    this->SpawnedBy = _SpawnedBy;
   
}


// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
    this->SetActorScale3D(FVector(0.5, 0.5, 0.5));
    //ProjectileLifeDel.BindUFunction(this, FName("ProjectileDie"));
   // GetWorld()->GetTimerManager().SetTimer(ProjectileLifeTimerHandle, ProjectileLifeDel, 0.5, false, 1);
}

void AProjectile::ProjectileDie() 
{
    
    GetWorld()->DestroyActor(this);
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor != this) {
        GetWorld()->DestroyActor(this);
    }
   
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (SpawnedBy)
    {
        Direction.Normalize();
        FVector NewLocation = this->GetActorLocation() + Direction * DeltaTime * 2000;
        this->SetActorLocation(NewLocation);
    }
}

