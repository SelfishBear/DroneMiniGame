// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/IDamageable.h"
#include "DronePawn.generated.h"

class UAmmoComponent;
class UHealthComponent;
class UArrowComponent;
class AProjectTile;
class UProjectTileObjectPool;
class ADroneTaskCharacter;
class USpringArmComponent;
class UBoxComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UFloatingPawnMovement;

UCLASS()
class DRONETASK_API ADronePawn : public APawn, public IIDamageable
{
	GENERATED_BODY()

public:
	ADronePawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* DroneMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* DroneCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFloatingPawnMovement* DroneFloatingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ShootPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAmmoComponent* AmmoComponent;

	UPROPERTY()
	ADroneTaskCharacter* OwnerCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectTile")
	TSubclassOf<AProjectTile> ProjectTileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DroneUI")
	TSubclassOf<UUserWidget> DroneUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DroneUI")
	UUserWidget* DroneUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	float FireRate = 0.2f;

	bool bIsShooting = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	           FVector NormalImpulse, const FHitResult& Hit);

	virtual void ApplyDamage(float DamageAmount) override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	int32 AmmoAmount;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);

	void LookUp(float Value);
	void TurnRight(float Value);

	void HandleShooting(float Value);

	void Shoot();

	UFUNCTION()
	void DeactivateDrone();

	FTimerHandle FireRateTimerHandle;
};
