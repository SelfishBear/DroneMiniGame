// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Turret.generated.h"

class ADronePawn;
class AProjectTile;
class UArrowComponent;
class UBoxComponent;
class UPawnSensingComponent;
class UProjectileMovementComponent;


UCLASS()
class DRONETASK_API ATurret : public APawn
{
	GENERATED_BODY()

public:
	ATurret();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* BoxRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* ShootPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Awareness")
	UPawnSensingComponent* PawnSens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectTile")
	TSubclassOf<AProjectTile> ProjectTileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float FireRate = 1.0f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void FindDrone(APawn* Pawn);


private:
	void Shoot();

	UPROPERTY()
	ADronePawn* Target = nullptr;

	FTimerHandle OnFireTimer;

	void StartShooting();
	void StopShooting();
};
