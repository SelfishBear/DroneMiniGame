// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret/Turret.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Drone/DronePawn.h"
#include "Drone/DroneHealth/HealthComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectTile/ProjectTile.h"
#include "Runtime/AIModule/Classes/Perception/PawnSensingComponent.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxRoot"));
	RootComponent = BoxRoot;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(RootComponent);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	PawnSens = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSens"));
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnActorDeath.AddDynamic(this, &ATurret::DestroyTurret);

	PawnSens->OnSeePawn.AddDynamic(this, &ATurret::FindDrone);
}

void ATurret::ApplyDamage(float DamageAmount)
{
	HealthComponent->CurrentHealth = HealthComponent->CurrentHealth - DamageAmount;
	HealthComponent->IsDead();
}

void ATurret::DestroyTurret()
{
	Destroy();
}

void ATurret::FindDrone(APawn* Pawn)
{
	if (Pawn)
	{
		ADronePawn* Drone = Cast<ADronePawn>(Pawn);
		if (Drone)
		{
			Target = Drone;
			FVector TurretLocation = GetActorLocation();
			FVector DroneLocation = Drone->GetActorLocation();
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(TurretLocation, DroneLocation);
			SetActorRotation(NewRotation);
			StartShooting();
		}
	}
}


void ATurret::Shoot()
{
	if (!Target)
	{
		StopShooting();
		return;
	}

	UWorld* World = GetWorld();

	if (World)
	{
		FVector Location = Arrow->GetComponentLocation();
		FRotator Rotation = Arrow->GetComponentRotation();

		FVector ArrowFW = Arrow->GetForwardVector();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AProjectTile* ProjectTile = World->SpawnActor<AProjectTile>(
			ProjectTileClass, Location, Rotation, SpawnParams);
		if (ProjectTile)
		{
			const float InitialSpeed = ProjectTile->GetInitialSpeed();

			UProjectileMovementComponent* ProjectileMovementComponent = ProjectTile->FindComponentByClass<
				UProjectileMovementComponent>();
			if (ProjectileMovementComponent)
			{
				ProjectileMovementComponent->Velocity = (InitialSpeed + 5000.0f) * ArrowFW;
			}
		}
	}
}

void ATurret::StartShooting()
{
	if (!GetWorldTimerManager().IsTimerActive(OnFireTimer))
	{
		GetWorldTimerManager().SetTimer(OnFireTimer, this, &ATurret::Shoot, FireRate, false);
	}
}

void ATurret::StopShooting()
{
	GetWorldTimerManager().ClearTimer(OnFireTimer);
}


void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
