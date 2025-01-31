// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneTask/Public/Drone/DronePawn.h"

#include "Drone/DroneHealth/HealthComponent.h"
#include "ProjectTile/ProjectTile.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Drone/DroneAmmo/AmmoComponent.h"
#include "DroneTask/DroneTaskCharacter.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/IPickable.h"


ADronePawn::ADronePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxRoot"));
	RootComponent = BoxRoot;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(RootComponent);

	DroneCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	DroneCamera->SetupAttachment(DroneMesh);
	DroneCamera->bUsePawnControlRotation = true;

	ShootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Shoot Position"));
	ShootPoint->SetupAttachment(DroneMesh);
	ShootPoint->SetRelativeLocation(FVector(-26.258468, -2.168485, -41.953629));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	AmmoComponent = CreateDefaultSubobject<UAmmoComponent>(TEXT("AmmoComponent"));

	DroneFloatingComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("DroneFloatingComponent"));
}


void ADronePawn::BeginPlay()
{
	Super::BeginPlay();

	BoxRoot->OnComponentHit.AddDynamic(this, &ADronePawn::OnHit);
	HealthComponent->OnDroneDeath.AddDynamic(this, &ADronePawn::DeactivateDrone);
}

void ADronePawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                       FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor) return;

	if (OtherActor->Implements<UIPickable>())
	{
		UE_LOG(LogTemp, Warning, TEXT("YES IMPLEMETS INTERFACE"));
		
		IIPickable* Pickable = Cast<IIPickable>(OtherActor);
		if (Pickable)
		{
			Pickable->PickUp(this);
		}
		
	}
}

void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HealthComponent->IsDead();
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
	if (AmmoComponent->CurrentAmmo > 0)
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
				AmmoComponent->CurrentAmmo--;
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
