// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/IDamageable.h"
#include "Turret.generated.h"

class UHealthComponent;
class ADronePawn;
class AProjectTile;
class UArrowComponent;
class UBoxComponent;
class UPawnSensingComponent;
class UProjectileMovementComponent;


UCLASS()
class DRONETASK_API ATurret : public APawn, public IIDamageable
{
	GENERATED_BODY()

public:
	ATurret();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* BoxRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Awareness")
	UPawnSensingComponent* PawnSens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectTile")
	TSubclassOf<AProjectTile> ProjectTileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float FireRate = 1.0f;

protected:
	virtual void BeginPlay() override;

	virtual void ApplyDamage(float DamageAmount) override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void FindDrone(APawn* Pawn);

private:
	void Shoot();

	FTransform InitialTransform;

	UPROPERTY()
	ADronePawn* Target = nullptr;

	FTimerHandle OnFireTimer;

	UFUNCTION()
	void DestroyTurret();

	void StartShooting();
	void StopShooting();
};
