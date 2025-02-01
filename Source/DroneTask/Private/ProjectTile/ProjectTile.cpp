// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectTile/ProjectTile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/IDamageable.h"

AProjectTile::AProjectTile()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootComponent = BoxRoot;
	BoxRoot->OnComponentHit.AddDynamic(this, &AProjectTile::NotifyHit);

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectTile"));
	TileMesh->SetupAttachment(RootComponent);
	TileMesh->SetSimulatePhysics(false);


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
	if (!OtherActor) return;
	if (OtherActor->Implements<UIDamageable>())
	{
		IIDamageable* Damageable = Cast<IIDamageable>(OtherActor);
		if (Damageable)
		{
			Damageable->ApplyDamage(TileDamage);
		}
	}
	Destroy();
}

void AProjectTile::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ReturnTimerHandle, this, &AProjectTile::DestroyProjectile, 3.0f, false);
}
