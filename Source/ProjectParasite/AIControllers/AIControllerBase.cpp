// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnShooter.h"
#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectParasite/BTTasks/BTTask_Chase.h"

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	enemyRef = Cast<APawnShooter>(GetPawn());

	blackboard->SetValueAsObject("PlayerRef", playerRef);
	
	SetCurrentState(currentState);
	
}

void AAIControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AAIControllerBase::SetCurrentState(EnemyStates state)
{
	blackboard->SetValueAsEnum("CurrentState", (uint8)state);
	currentState = state;
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	StartAIBehavior();
}

void AAIControllerBase::StartAIBehavior()
{
	if(behaviorTree != nullptr)
	{
		RunBehaviorTree(behaviorTree);
		blackboard = GetBlackboardComponent();
	}
}