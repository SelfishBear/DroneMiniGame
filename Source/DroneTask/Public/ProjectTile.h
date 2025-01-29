// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectTile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class DRONETASK_API AProjectTile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectTile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TileMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* TileMovementComponent;
	
	void DestroyProjectile();

	float GetInitialSpeed();

protected:
	UFUNCTION()
	void NotifyHit(
			UPrimitiveComponent* HitComp, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			FVector NormalImpulse, 
			const FHitResult& Hit
		);

	virtual void BeginPlay() override;
	
private:
	
	
	float MaxSpeed = 2000.0f;
	float InitialSpeed = MaxSpeed;

	FTimerHandle ReturnTimerHandle;
	
};
