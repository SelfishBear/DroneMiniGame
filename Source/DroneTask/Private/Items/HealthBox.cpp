// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HealthBox.h"

#include "Drone/DroneHealth/HealthComponent.h"

// Sets default values
AHealthBox::AHealthBox()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	HealthMesh = CreateDefaultSubobject<UStaticMeshComponent>("HealthMesh");
	HealthMesh->SetupAttachment(Root);
}

void AHealthBox::BeginPlay()
{
	Super::BeginPlay();
}

void AHealthBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHealthBox::PickUp(AActor* Picker)
{
	if (!Picker) return;

	UHealthComponent* HealthComponent = Picker->FindComponentByClass<UHealthComponent>();
	if (HealthComponent)
	{
		HealthComponent->CurrentHealth = FMath::Clamp(HealthComponent->CurrentHealth + HealAmount, 0, 100);
		Destroy();
	}
}
