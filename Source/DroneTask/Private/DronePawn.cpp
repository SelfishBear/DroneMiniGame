// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneTask/Public/DronePawn.h"

#include "ProjectTile.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "DroneTask/DroneTaskCharacter.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ADronePawn::ADronePawn()
{
	PrimaryActorTick.bCanEverTick = true;


	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	RootComponent = DefaultSceneRoot;

	DroneCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	DroneCamera->SetupAttachment(RootComponent);
	DroneCamera->bUsePawnControlRotation = true;

	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(DroneCamera);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	BoxCollision->SetupAttachment(DroneMesh);

	ShootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Shoot Position"));
	ShootPoint->SetupAttachment(DroneMesh);
	ShootPoint->SetRelativeLocation(FVector(-26.258468, -2.168485, -41.953629));

	DroneFloatingComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("DroneFloatingComponent"));
}

void ADronePawn::BeginPlay()
{
	Super::BeginPlay();

	if (DroneUIClass)
        {
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
		
			UUserWidget* DroneUI = CreateWidget<UUserWidget>(PC, DroneUIClass);
    
            if (DroneUI)
            {
                DroneUI->AddToViewport();
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("DroneUIClass is not assigned!"));
        }
}


void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADronePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADronePawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ADronePawn::MoveUp);
	PlayerInputComponent->BindAxis("LookUp", this, &ADronePawn::LookUp);
	PlayerInputComponent->BindAxis("TurnRight", this, &ADronePawn::TurnRight);
	PlayerInputComponent->BindAxis("Shoot", this, &ADronePawn::HandleShooting);
	PlayerInputComponent->BindAction("DeactivateDrone", IE_Pressed, this, &ADronePawn::DeactivateDrone);
}

void ADronePawn::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ADronePawn::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ADronePawn::MoveUp(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorUpVector(), Value);
	}
}

void ADronePawn::LookUp(float Value)
{
	AddControllerYawInput(Value);
}

void ADronePawn::TurnRight(float Value)
{
	AddControllerPitchInput(Value);
}


void ADronePawn::HandleShooting(float Value)
{
	if (Value > 0.0f)
	{
		if (!bIsShooting)
		{
			bIsShooting = true;
			Shoot(); // Делаем первый выстрел сразу
			GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ADronePawn::Shoot, FireRate, true);
		}
	}
	else
	{
		if (bIsShooting)
		{
			bIsShooting = false;
			GetWorldTimerManager().ClearTimer(FireRateTimerHandle);
		}
	}
}


void ADronePawn::Shoot()
{
	if (AmmoAmount > 0)
	{
		UWorld* World = GetWorld();

		if (World)
		{
			FVector Location = ShootPoint->GetComponentLocation();
			FRotator Rotation = DroneCamera->GetComponentRotation();

			FVector CameraForwardVector = DroneCamera->GetForwardVector();

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AProjectTile* ProjectTile = World->SpawnActor<AProjectTile>(ProjectTileClass, Location, Rotation,
			                                                            SpawnParams);

			if (ProjectTile)
			{
				const float InitialSpeed = ProjectTile->GetInitialSpeed();

				UProjectileMovementComponent* ProjectileMovementComponent = ProjectTile->FindComponentByClass<
					UProjectileMovementComponent>();
				if (ProjectileMovementComponent)
				{
					ProjectileMovementComponent->Velocity = InitialSpeed * CameraForwardVector;
				}
				AmmoAmount--;
			}
		}
	}
}


void ADronePawn::DeactivateDrone()
{
	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;
	

	if (OwnerCharacter)
	{
		PlayerController->Possess(OwnerCharacter);
		OwnerCharacter->bCanActivateDrone = true;
		Destroy();
	}

	UE_LOG(LogTemp, Error, TEXT("DEACTIVATE DRONE"));
}
