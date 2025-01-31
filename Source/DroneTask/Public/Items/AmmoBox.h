// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IPickable.h"
#include "AmmoBox.generated.h"

UCLASS()
class DRONETASK_API AAmmoBox : public AActor, public IIPickable
{
	GENERATED_BODY()
	
public:	
	AAmmoBox();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BoxMesh;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void PickUp(AActor* Picker) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo", meta =(ClampMin = 0, ClampMax = 75))
	int32 AmmoAmount;
};
