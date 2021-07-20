// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnShooter.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/GameStateCustom.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectParasite/BTTasks/BTTask_Chase.h"
#include "ProjectParasite/GameManager.h"

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	gameManagerRef = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	gameStateRef = Cast<AGameStateCustom>(UGameplayStatics::GetGameState(GetWorld()));
	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	StartAIBehavior();
	
	blackboard->SetValueAsObject("PlayerRef", playerRef);
}

void AAIControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AAIControllerBase::SetCurrentState(EnemyStates state)
{
	UE_LOG(LogTemp, Warning, TEXT("%i"), state);
	blackboard->SetValueAsEnum("CurrentState", (uint8)state);
	currentState = state;
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	StartAIBehavior();

	enemyRef = Cast<APawnEnemy>(InPawn);
	
	if(enemyRef)
	{
		if(enemyRef->GetIsStationary())
			SetCurrentState(EnemyStates::State_Guard);
		else
			SetCurrentState(EnemyStates::State_Patrol);	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AIController %s is not controlling any enemy"), *GetName());
	}
}

void AAIControllerBase::StartAIBehavior()
{
	if(behaviorTree != nullptr)
	{
		RunBehaviorTree(behaviorTree);
		blackboard = GetBlackboardComponent();
	}
}