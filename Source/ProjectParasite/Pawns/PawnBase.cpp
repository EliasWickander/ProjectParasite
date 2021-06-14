// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnBase.h"

#include "PawnParasite.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ProjectParasite/PlayerControllers/PlayerControllerBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/DefaultPawn.h"

APawnBase::APawnBase()
{
	PrimaryActorTick.bCanEverTick = true;

	capsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = capsuleCollider;

	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	baseMesh->SetupAttachment(RootComponent);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);

	movementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(ADefaultPawn::MovementComponentName);
	movementComponent->UpdatedComponent = capsuleCollider;

	//Remove this later
	springArm->bInheritPitch = false;
	springArm->bInheritYaw = false;
	springArm->bInheritRoll = false;
}

void APawnBase::BeginPlay()
{
	Super::BeginPlay();

	floatingPawnMovement = FindComponentByClass<UFloatingPawnMovement>();
	floatingPawnMovement->MaxSpeed = moveSpeed;

	playerControllerRef = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController());
	OnTakeAnyDamage.AddDynamic(this, &APawnBase::OnTakeDamage);
	
	currentHealth = maxHealth;

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *playerControllerRef->GetName())
}

void APawnBase::MoveVertical(float axis)
{
	if(!canMove)
		return;
	
	AddMovementInput(FVector::ForwardVector * axis);
}

void APawnBase::MoveHorizontal(float axis)
{
	if(!canMove)
		return;
	
	AddMovementInput(FVector::RightVector * axis);
}

//Rotate actor towards target point on the yaw only
void APawnBase::Rotate(FVector targetPoint)
{
	//Get dir to rotate in
	targetPoint.Z = GetActorLocation().Z;
	
	FVector rotationDir = targetPoint - GetActorLocation();
	rotationDir.Normalize();

	//Rotate
	FRotator targetRotation = rotationDir.Rotation();

	SetActorRotation(targetRotation);
}

void APawnBase::RotateToMouseCursor()
{
	FHitResult hitResult;

	//If the ray-cast to cursor hit something, rotate to hit location
	if(RaycastToMouseCursor(hitResult))
	{
		Rotate(hitResult.Location);
	}
}

bool APawnBase::RaycastToMouseCursor(FHitResult& hitResult)
{
	//Do a ray-cast below cursor

	playerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, true, hitResult);

	AActor* actorHit = hitResult.GetActor();
	
	return actorHit != nullptr;
}

void APawnBase::SetMoveSpeed(float speed)
{
	moveSpeed = speed;
	floatingPawnMovement->MaxSpeed = moveSpeed;
	UE_LOG(LogTemp, Warning, TEXT("%f"), floatingPawnMovement->MaxSpeed);
}

float APawnBase::GetMoveSpeed()
{
	return moveSpeed;
}

void APawnBase::SetCanMove(bool enabled)
{
	canMove = enabled;
}

bool APawnBase::GetCanMove()
{
	return canMove;
}

void APawnBase::OnTakeDamage(AActor* damagedActor, float damage, const UDamageType* damageType, AController* causerController,
	AActor* causerActor)
{
	currentHealth -= damage;

	UE_LOG(LogTemp, Warning, TEXT("Took %f damage"), damage);
	if(currentHealth <= 0)
	{
		//Die
		Destroy();
	}
}
