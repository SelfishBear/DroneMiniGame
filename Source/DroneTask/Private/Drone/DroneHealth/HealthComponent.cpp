// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone/DroneHealth/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UHealthComponent::IsDead()
{
	if (CurrentHealth <= 0.0f)
	{
		OnActorDeath.Broadcast();
		return true;
	}
	return false;
}


