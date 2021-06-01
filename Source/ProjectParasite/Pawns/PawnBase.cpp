// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnBase.h"

#include "PawnParasite.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ProjectParasite/PlayerControllers/ParasitePlayerController.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = capsuleCollider;
	
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	baseMesh->SetupAttachment(RootComponent);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(RootComponent);
	
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);

	springArm->bInheritPitch = false;
	springArm->bInheritYaw = false;
	springArm->bInheritRoll = false;

}

void APawnBase::BeginPlay()
{
	Super::BeginPlay();
	playerControllerRef = Cast<AParasitePlayerController>(GetWorld()->GetFirstPlayerController());
}

void APawnBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MoveInInputDirection();

	if(IsPlayerControlled())
		RotateToMouseCursor();
}

void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveHorizontal", this, &APawnBase::CalculateMovementHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &APawnBase::CalculateMovementVertical);	
}



//Move actor in direction (world space)
void APawnBase::Move(FVector moveDir)
{
	AddActorWorldOffset(moveDir * moveSpeed * GetWorld()->GetDeltaSeconds(), true);
}

//Rotate actor towards target point on the yaw only
void APawnBase::Rotate(FVector targetPoint)
{
	targetPoint.Z = GetActorLocation().Z;
	
	FVector rotationDir = targetPoint - GetActorLocation();
	rotationDir.Normalize();

	FRotator targetRotation = rotationDir.Rotation();

	SetActorRotation(targetRotation);
}

void APawnBase::CalculateMovementHorizontal(float axis)
{	
	if(IsPlayerControlled())
	{
		horizontalAxis = axis;	
	}
	else
	{
		horizontalAxis = 0;
	}
}

void APawnBase::CalculateMovementVertical(float axis)
{
	if(IsPlayerControlled())
	{
		verticalAxis = axis;	
	}
	else
	{
		verticalAxis = 0;
	}
}

void APawnBase::MoveInInputDirection()
{
	FVector moveDir(verticalAxis, horizontalAxis, 0);
	moveDir.Normalize();
	
	Move(moveDir);
}

void APawnBase::RotateToMouseCursor()
{
	//Do a ray-cast below cursor
	FHitResult hitResult;

	playerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, true, hitResult);

	AActor* actorHit = hitResult.GetActor();

	//If there's a hit, rotate to hit location
	if(actorHit)
	{
		Rotate(hitResult.Location);
	}
}