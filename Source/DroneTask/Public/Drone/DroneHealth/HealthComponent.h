// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorDeath);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DRONETASK_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta=(ClampMin = 0.0f, ClampMax = 100.0f))
	float CurrentHealth = 0;

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealth() { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category="Health")
	bool IsDead();

	FOnActorDeath OnActorDeath;
};
