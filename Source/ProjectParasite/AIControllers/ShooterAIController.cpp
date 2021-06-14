// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnShooter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"

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
