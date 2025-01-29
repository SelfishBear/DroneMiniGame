// Copyright Epic Games, Inc. All Rights Reserved.

#include "DroneTaskCharacter.h"
#include "DroneTaskProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Public/DronePawn.h"

DEFINE_LOG_CATEGORY_STATIC(ADroneTaskCharacterLog, All, All);

ADroneTaskCharacter::ADroneTaskCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	DroneSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("DroneSpawnPoint"));
	DroneSpawnPoint->SetupAttachment(GetCapsuleComponent());
	

}

void ADroneTaskCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ADroneTaskCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADroneTaskCharacter::Move);
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADroneTaskCharacter::Look);
		
		EnhancedInputComponent->BindAction(ActivateDroneAction, ETriggerEvent::Started, this, &ADroneTaskCharacter::ActivateDrone);
		
	}
	else
	{
		UE_LOG(ADroneTaskCharacterLog, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADroneTaskCharacter::ActivateDrone()
{
	if (!bCanActivateDrone)
	{
		UE_LOG(LogTemp, Warning, TEXT("Drone exists"));
		return;
	}
	
	UWorld* World = GetWorld();
	if (!World || !DroneSpawnPoint) return;
	
	FVector SpawnPointTransform = 	DroneSpawnPoint->GetComponentLocation();
	FRotator SpawnPointRotation = DroneSpawnPoint->GetComponentRotation();
	

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =  ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADronePawn* DronePawn = World->SpawnActor<ADronePawn>(BPDrone, SpawnPointTransform, SpawnPointRotation, SpawnParams);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	if (DronePawn && PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Дрон заспавнен в: %s"), *SpawnPointTransform.ToString());

		DronePawn->OwnerCharacter = this;
		
		PlayerController->Possess(DronePawn);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Ошибка спавна дрона!"));
	}
	bCanActivateDrone = false;
}


void ADroneTaskCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ADroneTaskCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}