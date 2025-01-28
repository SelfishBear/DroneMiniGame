// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneTask/Public/DronePawn.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ADronePawn::ADronePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	RootComponent = DefaultSceneRoot;
	
	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(DroneMesh);

	
	DroneCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	DroneCamera->SetupAttachment(SpringArm);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	BoxCollision->SetupAttachment(DroneMesh);
	
	DroneFloatingComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("DroneFloatingComponent"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/DroneAssets/MilitaryDrone/Meshes/drone_model.drone_model'"));
	if (MeshAsset.Succeeded())
	{
		DroneMesh->SetSkeletalMesh(MeshAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Не удалось загрузить меш"));
	}
}

// Called when the game starts or when spawned
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADronePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADronePawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ADronePawn::MoveUp);
	PlayerInputComponent->BindAxis("LookUp", this, &ADronePawn::LookUp);
	PlayerInputComponent->BindAxis("TurnRight", this, &ADronePawn::TurnRight);

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
	AddControllerPitchInput(Value);
}

void ADronePawn::TurnRight(float Value)
{
	AddControllerYawInput(Value);
}

