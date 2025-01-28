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

DEFINE_LOG_CATEGORY(LogTemplateCharacter);
DEFINE_LOG_CATEGORY_STATIC(SHISH, All, All);

//////////////////////////////////////////////////////////////////////////
// ADroneTaskCharacter

ADroneTaskCharacter::ADroneTaskCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	DroneSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("DroneSpawnPoint"));
	DroneSpawnPoint->SetupAttachment(GetCapsuleComponent());
	DroneSpawnPoint->SetRelativeLocation(FVector(233.0, 143.0, 128.0 ));

}

//////////////////////////////////////////////////////////////////////////// Input

void ADroneTaskCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
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
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADroneTaskCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADroneTaskCharacter::Look);
		
		EnhancedInputComponent->BindAction(ActivateDroneAction, ETriggerEvent::Started, this, &ADroneTaskCharacter::ActivateDrone);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADroneTaskCharacter::ActivateDrone()
{
	UWorld* World = GetWorld();
	if (!World || !DroneSpawnPoint) return;
	
	FVector SpawnPointTransform = 	DroneSpawnPoint->GetRelativeLocation();
	FRotator SpawnPointRotation = DroneSpawnPoint->GetComponentRotation();
	
	SpawnPointTransform = GetActorTransform().TransformPosition(SpawnPointTransform);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =  ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADronePawn* DronePawn = World->SpawnActor<ADronePawn>(ADronePawn::StaticClass(), SpawnPointTransform, SpawnPointRotation.Add(0.0f,-90.0f, 0.0f), SpawnParams);

	if (DronePawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Дрон заспавнен в: %s"), *SpawnPointTransform.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Ошибка спавна дрона!"));
	}
}


void ADroneTaskCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
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
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}