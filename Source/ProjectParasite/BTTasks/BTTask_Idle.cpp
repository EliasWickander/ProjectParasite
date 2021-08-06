// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Idle.h"

#include "AIController.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/AIControllers/AIControllerBase.h"

UBTTask_Idle::UBTTask_Idle()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BTTaskIdleMemory* instanceMemory = reinterpret_cast<BTTaskIdleMemory*>(NodeMemory);

	instanceMemory->ownerEnemy = Cast<APawnEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	instanceMemory->ownerAIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());

	instanceMemory->ownerAIController->ClearFocus(EAIFocusPriority::Gameplay);
	instanceMemory->ownerAIController->StopMovement();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTTask_Idle::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

uint16 UBTTask_Idle::GetInstanceMemorySize() const
{
	return sizeof(BTTaskIdleMemory);
}