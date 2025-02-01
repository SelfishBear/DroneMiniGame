// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/AmmoBox.h"
#include "Drone/DroneAmmo/AmmoComponent.h"

AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>("BoxMesh");
	BoxMesh->SetupAttachment(RootComponent);
}

void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();
}

void AAmmoBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAmmoBox::PickUp(AActor* Picker)
{
	if (!Picker) return;
	UAmmoComponent* AmmoComponent = Picker->FindComponentByClass<UAmmoComponent>();
	if (AmmoComponent)
	{
		AmmoComponent->CurrentAmmo = FMath::Clamp(AmmoComponent->CurrentAmmo + AmmoAmount, 0, 1000);
		Destroy();
	}
}
