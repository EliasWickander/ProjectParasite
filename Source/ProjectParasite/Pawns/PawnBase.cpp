// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnBase.h"

#include "PawnParasite.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ProjectParasite/PlayerControllers/ParasitePlayerController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

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

	movementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(ADefaultPawn::MovementComponentName);
	movementComponent->UpdatedComponent = capsuleCollider;

	springArm->bInheritPitch = false;
	springArm->bInheritYaw = false;
	springArm->bInheritRoll = false;

	OnTakeAnyDamage.AddDynamic(this, &APawnBase::TakeDamage);
}

void APawnBase::BeginPlay()
{
	Super::BeginPlay();
	playerControllerRef = Cast<AParasitePlayerController>(GetWorld()->GetFirstPlayerController());

	currentHealth = maxHealth;
}

void APawnBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveHorizontal", this, &APawnBase::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &APawnBase::MoveVertical);	
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
	targetPoint.Z = GetActorLocation().Z;
	
	FVector rotationDir = targetPoint - GetActorLocation();
	rotationDir.Normalize();

	FRotator targetRotation = rotationDir.Rotation();

	SetActorRotation(targetRotation);
}

void APawnBase::Rotate(FVector targetPoint, float speed)
{
	targetPoint.Z = GetActorLocation().Z;
	
	FVector rotationDir = targetPoint - GetActorLocation();
	rotationDir.Normalize();

	FRotator targetRotation = rotationDir.Rotation();

	FRotator lerpedRotation = FMath::Lerp(GetActorRotation(), targetRotation, speed * GetWorld()->DeltaTimeSeconds);

	SetActorRotation(lerpedRotation);
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

void APawnBase::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* i, AActor* DamageCauser)
{
	currentHealth -= Damage;

	UE_LOG(LogTemp, Warning, TEXT("Took %f damage"), Damage);
	if(currentHealth <= 0)
	{
		//Die
		Destroy();
	}
}
