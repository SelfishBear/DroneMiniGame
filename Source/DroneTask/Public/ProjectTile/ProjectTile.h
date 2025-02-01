// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectTile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class DRONETASK_API AProjectTile : public AActor
{
	GENERATED_BODY()

public:
	AProjectTile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TileMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* TileMovementComponent;

	void DestroyProjectile();

	float GetInitialSpeed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileDamage", meta=(AllowPrivateAccess = "true"))
	float TileDamage = 30.0f;

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
	float MaxSpeed = 5000.0f;
	float InitialSpeed = MaxSpeed;

	FTimerHandle ReturnTimerHandle;
};
