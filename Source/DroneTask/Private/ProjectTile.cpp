// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectTile.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectTile::AProjectTile()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectTile"));
	TileMesh->SetupAttachment(RootComponent);
	TileMesh->SetSimulatePhysics(false);
	TileMesh->OnComponentHit.AddDynamic(this, &AProjectTile::NotifyHit);

	TileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	TileMovementComponent->InitialSpeed = InitialSpeed;
	TileMovementComponent->MaxSpeed = MaxSpeed;
	TileMovementComponent->bShouldBounce = false;
}

void AProjectTile::DestroyProjectile()
{
	Destroy();
}

float AProjectTile::GetInitialSpeed()
{
	return InitialSpeed;
}

void AProjectTile::NotifyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AProjectTile::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ReturnTimerHandle, this, &AProjectTile::DestroyProjectile, 3.0f, false);
}






