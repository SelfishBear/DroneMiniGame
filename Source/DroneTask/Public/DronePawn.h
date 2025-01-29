// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"
#include "DronePawn.generated.h"

class AProjectTile;
class UProjectTileObjectPool;
class ADroneTaskCharacter;
class USpringArmComponent;
class UBoxComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UFloatingPawnMovement;

UCLASS()
class DRONETASK_API ADronePawn : public APawn
{
	GENERATED_BODY()

public:
	ADronePawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* DroneMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* DroneCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFloatingPawnMovement* DroneFloatingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ShootPoint;
	
	UPROPERTY()
	ADroneTaskCharacter* OwnerCharacter = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectTile")
	TSubclassOf<AProjectTile> ProjectTileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DroneUI")
	TSubclassOf<UUserWidget> DroneUIClass;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	float FireRate = 0.2f;

	bool bIsShooting = false;
	
protected:
	virtual void BeginPlay() override;


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	int32 AmmoAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	
	void LookUp(float Value);
	void TurnRight(float Value);

	void HandleShooting(float Value);
	
	void Shoot();

	void DeactivateDrone();

	FTimerHandle FireRateTimerHandle;
	
};
