// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SearchNoise.h"
#include "AIController.h"
#include "ProjectParasite/AIControllers/AIControllerBase.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Utilities/DevUtils.h"
UBTTask_SearchNoise::UBTTask_SearchNoise()
{
	NodeName = TEXT("Search Noise");

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_SearchNoise::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BTTaskSearchNoiseMemory* instanceMemory = reinterpret_cast<BTTaskSearchNoiseMemory*>(NodeMemory);

	instanceMemory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();
	instanceMemory->reactionTimer = 0;
	instanceMemory->lookAroundTimer = 0;

	instanceMemory->ownerEnemy->SetMoveSpeed(instanceMemory->ownerEnemy->GetChaseSpeed());

	UE_LOG(LogTemp, Warning, TEXT("Noise Start"));
	return EBTNodeResult::InProgress;
}

void UBTTask_SearchNoise::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskSearchNoiseMemory* instanceMemory = reinterpret_cast<BTTaskSearchNoiseMemory*>(NodeMemory);

	AAIControllerBase* AIController = instanceMemory->ownerEnemy->GetAIController();
	APawnEnemy* ownerEnemy = instanceMemory->ownerEnemy;
	
	if(instanceMemory->reactionTimer < ownerEnemy->GetNoiseReactionTime())
	{
		instanceMemory->reactionTimer += DeltaSeconds;
	}
	else
	{
		bool finishedRotating = SetFocalPointExtended(AIController, ownerEnemy->GetLastHeardNoisePos(), ownerEnemy->GetTurnRate(), 0.2f);

		if(finishedRotating)
		{
			AIController->MoveToLocation(instanceMemory->ownerEnemy->GetLastHeardNoisePos());
			instanceMemory->reachedGoal = false;
		}
	}

	if(AIController->GetPathFollowingComponent()->DidMoveReachGoal())
	{
		instanceMemory->reachedGoal = true;
	}

	if(instanceMemory->reachedGoal)
	{
		if(instanceMemory->reactionTimer < 1)
		{
			instanceMemory->reactionTimer += DeltaSeconds;
		}
		else
		{
			AIController->SetCurrentState(EnemyStates::State_Patrol);
		}
	}
}

void UBTTask_SearchNoise::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

uint16 UBTTask_SearchNoise::GetInstanceMemorySize() const
{
	return sizeof(BTTaskSearchNoiseMemory);
}
