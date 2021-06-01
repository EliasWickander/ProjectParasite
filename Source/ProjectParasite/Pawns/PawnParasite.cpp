// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnParasite.h"

#include "DrawDebugHelpers.h"
#include "ProjectParasite/PlayerControllers/ParasitePlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

APawnParasite::APawnParasite()
{
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(RootComponent);
	
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
}

// Called when the game starts or when spawned
void APawnParasite::BeginPlay()
{
	
	playerControllerRef = Cast<AParasitePlayerController>(GetWorld()->GetFirstPlayerController());

	if(playerControllerRef)
	{
		playerControllerRef->SetPlayerInputEnabled(true);
	}
	
	Super::BeginPlay();
	
}

// Called every frame
void APawnParasite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveInInputDirection();
	RotateToMouseCursor();

}

// Called to bind functionality to input
void APawnParasite::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveHorizontal", this, &APawnParasite::CalculateMovementHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &APawnParasite::CalculateMovementVertical);

}

void APawnParasite::MoveInInputDirection()
{
	FVector moveDir(verticalAxis, horizontalAxis, 0);
	moveDir.Normalize();
	
	Move(moveDir);
}

void APawnParasite::RotateToMouseCursor()
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

void APawnParasite::CalculateMovementHorizontal(float axis)
{
	horizontalAxis = axis;
}

void APawnParasite::CalculateMovementVertical(float axis)
{
	verticalAxis = axis;
}