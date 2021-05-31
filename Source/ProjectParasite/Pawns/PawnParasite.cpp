// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnParasite.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APawnParasite::APawnParasite()
{
	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	RootComponent = sphereCollider;

	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	baseMesh->SetupAttachment(RootComponent);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
}

// Called when the game starts or when spawned
void APawnParasite::BeginPlay()
{
	Super::BeginPlay();
	
}

void APawnParasite::CalculateMovementHorizontal(float axis)
{
	horizontalAxis = axis;
}

void APawnParasite::CalculateMovementVertical(float axis)
{
	verticalAxis = axis;
}

// Called every frame
void APawnParasite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector moveDir(verticalAxis, horizontalAxis, 0);
	moveDir.Normalize();
	
	Move(moveDir);

}

// Called to bind functionality to input
void APawnParasite::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveHorizontal", this, &APawnParasite::CalculateMovementHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &APawnParasite::CalculateMovementVertical);

}