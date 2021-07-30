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
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "Components/PostProcessComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Kismet/GameplayStatics.h"
APawnBase::APawnBase()
{
	PrimaryActorTick.bCanEverTick = true;

	capsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = capsuleCollider;

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	skeletalMesh->SetupAttachment(RootComponent);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(RootComponent);

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->SetupAttachment(springArm);

	postProcessComponent = CreateDefaultSubobject<UPostProcessComponent>("Post Process Component");
	postProcessComponent->SetupAttachment(cameraComponent);

	movementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(ADefaultPawn::MovementComponentName);
	movementComponent->UpdatedComponent = capsuleCollider;

	pawnNoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Pawn Noise Emitter"));

	//Remove this later
	springArm->bInheritPitch = false;
	springArm->bInheritYaw = false;
	springArm->bInheritRoll = false;

	bUseControllerRotationYaw = true;
}

void APawnBase::BeginPlay()
{
	Super::BeginPlay();

	floatingPawnMovement = FindComponentByClass<UFloatingPawnMovement>();

	playerControllerRef = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController());
	OnTakeAnyDamage.AddDynamic(this, &APawnBase::OnTakeDamage);
	onStartDeathEvent.AddDynamic(this, &APawnBase::OnDeath);
	
	currentHealth = maxHealth;
}

void APawnBase::Tick(float DeltaSeconds)
{	
	Super::Tick(DeltaSeconds);

	if(isPendingDeath)
	{
		HandlePendingDeath();
	}
	
	UpdatePawnBehavior(DeltaSeconds);
}

void APawnBase::UpdatePawnBehavior(float deltaSeconds)
{

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
	currentMoveSpeed = speed;
	floatingPawnMovement->MaxSpeed = currentMoveSpeed;
}

void APawnBase::HandlePendingDeath()
{
	//Let body remain post-death for some time
	if(deathTimer < deathTime)
	{
		deathTimer += GetWorld()->DeltaTimeSeconds;
	}
	else
	{
		//Destroy
		if(Cast<APawnParasite>(this))
		{
			
		}
		else
		{
			Destroy();			
		}
	}
}

void APawnBase::OnTakeDamage(AActor* damagedActor, float damage, const UDamageType* damageType, AController* causerController,
                             AActor* causerActor)
{
	if(Cast<APawnParasite>(damagedActor) && causerActor == playerControllerRef->GetPlayer()->GetPossessedEnemy())
		return;
	
	currentHealth -= damage;

	if(Cast<APawnEnemy>(causerActor))
	{
		OnDamagedByCharacter();
	}
	
	if(currentHealth <= 0)
	{
		isPendingDeath = true;
		onStartDeathEvent.Broadcast(this, causerActor);
	}
}

void APawnBase::ReportNoise(USoundBase* soundToPlay, float volume)
{
	if(soundToPlay != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundToPlay, GetActorLocation(), volume);
 
	}
	MakeNoise(volume, this, GetActorLocation());
}

void APawnBase::OnDeath(APawnBase* deadPawn, AActor* causerActor)
{
	
}

