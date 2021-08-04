// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LookAround.h"

#include "AIController.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"

UBTTask_LookAround::UBTTask_LookAround()
{
	NodeName = TEXT("Look Around");

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_LookAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BTTaskLookAroundMemory* instanceMemory = reinterpret_cast<BTTaskLookAroundMemory*>(NodeMemory);

	//if this is the first time this task is executed (to save current waypoint if patrolling is interrupted)
	if(instanceMemory->ownerEnemy == nullptr)
	{
		instanceMemory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();	
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_LookAround::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	BTTaskLookAroundMemory* instanceMemory = reinterpret_cast<BTTaskLookAroundMemory*>(NodeMemory);
}

void UBTTask_LookAround::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	BTTaskLookAroundMemory* instanceMemory = reinterpret_cast<BTTaskLookAroundMemory*>(NodeMemory);
}

uint16 UBTTask_LookAround::GetInstanceMemorySize() const
{
	return sizeof(BTTaskLookAroundMemory);
}
