// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IPickable.h"
#include "HealthBox.generated.h"

UCLASS()
class DRONETASK_API AHealthBox : public AActor, public IIPickable
{
	GENERATED_BODY()
	
public:	
	AHealthBox();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HealthMesh;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void PickUp(AActor* Picker) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta=(AllowPrivateAccess=true))
	float HealAmount = 35.0f;
	
};
