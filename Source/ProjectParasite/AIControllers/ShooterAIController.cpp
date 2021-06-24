// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnShooter.h"
#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectParasite/BTTasks/BTTask_Chase.h"


void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	shooterRef = Cast<APawnShooter>(GetPawn());
	
	SetCurrentState(ShooterStates::State_Patrol);
}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	blackboard->SetValueAsObject("PlayerRef", playerRef);
}

void AShooterAIController::SetCurrentState(ShooterStates state)
{
	blackboard->SetValueAsEnum("CurrentState", (uint8)state);
	currentState = state;
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	StartAIBehavior();
}

void AShooterAIController::StartAIBehavior()
{
	if(behaviorTree != nullptr)
	{
		RunBehaviorTree(behaviorTree);
	    blackboard = GetBlackboardComponent();
	}
}