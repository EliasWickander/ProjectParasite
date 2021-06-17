// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnShooter.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Utilities/DevUtils.h"
#include "Components/CapsuleComponent.h"

#include "BehaviorTree/BlackboardComponent.h"


void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	shooterRef = Cast<APawnShooter>(GetPawn());
}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Detect();
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetFocus(playerRef);
	StartAIBehavior();
	
}

void AShooterAIController::OnUnPossess()
{
	Super::OnUnPossess();

    //Isn't called it seems
	UE_LOG(LogTemp, Warning, TEXT("Unpossess"));
	StopMovement();

	ClearFocus(EAIFocusPriority::Gameplay);
}

void AShooterAIController::StartAIBehavior()
{
	if(behaviorTree != nullptr)
	{
		RunBehaviorTree(behaviorTree);
		blackboard = GetBlackboardComponent();
	}
}

void AShooterAIController::Detect()
{
	FBoxSphereBounds enemyColBounds = shooterRef->GetCollider()->Bounds;

	SCone coneData {
		enemyColBounds.Origin,
		shooterRef->GetActorRotation().Quaternion(),
		shooterRef->GetDetectionAngle(),
		enemyColBounds.BoxExtent.Z * 2,
		shooterRef->GetDetectionRange()
	};

	const TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	const TArray<AActor*> actorsToIgnore { };
	UClass* classFilter = APawnParasite::StaticClass();
	TArray<AActor*> outActors;
	
	if(OverlapCone(coneData, GetWorld(), objectTypes, classFilter, actorsToIgnore, outActors))
	{
		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(shooterRef);

		if(GetWorld()->LineTraceSingleByChannel(hitResult,shooterRef->GetActorLocation(),outActors[0]->GetActorLocation(), ECC_Visibility, params))
		{
			if(hitResult.GetActor() == playerRef)
			{
				blackboard->SetValueAsVector("PlayerLocation", outActors[0]->GetActorLocation());
				SetFocus(outActors[0]);		
			}
			else
			{
				//Vision obstructed by something in player's path
				blackboard->ClearValue("PlayerLocation");
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Didn't hit"));
		}
	}
	else
	{
		blackboard->ClearValue("PlayerLocation");
	}
}