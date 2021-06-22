// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Chase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "ProjectParasite/AIControllers/ShooterAIController.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Pawns/PawnParasite.h"

UBTTask_Chase::UBTTask_Chase()
{
	NodeName = TEXT("Chase");

	bNotifyTick = true;
	bNotifyTaskFinished = true;

	//Only accept actor blackboard key
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Chase, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BTTaskChaseMemory* memory = reinterpret_cast<BTTaskChaseMemory*>(NodeMemory);
	
	memory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();
	memory->blackboard = OwnerComp.GetBlackboardComponent();

	playerRef = memory->ownerEnemy->GetPlayerRef();
	
	if(playerRef->GetPossessedEnemy() != nullptr)
	{
		SetTarget(NodeMemory, playerRef->GetPossessedEnemy());
	}
	else
	{
		SetTarget(NodeMemory, playerRef);
	}

	memory->ownerEnemy->SetMoveSpeed(memory->ownerEnemy->GetChaseSpeed());
	return EBTNodeResult::InProgress;
}

void UBTTask_Chase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskChaseMemory* memory = reinterpret_cast<BTTaskChaseMemory*>(NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();

	if(targetActor != nullptr)
	{
		AIController->MoveToActor(targetActor, memory->ownerEnemy->GetAttackRange());

		if(AIController->GetPathFollowingComponent()->DidMoveReachGoal())
		{
			memory->blackboard->SetValueAsEnum("CurrentState", (uint8)ShooterStates::State_Attack);
		}
	}
}

void UBTTask_Chase::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{

	OwnerComp.GetAIOwner()->StopMovement();
	OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

uint16 UBTTask_Chase::GetInstanceMemorySize() const
{
	return sizeof(BTTaskChaseMemory);
}

void UBTTask_Chase::SetTarget(uint8* NodeMemory, AActor* target)
{
	BTTaskChaseMemory* instanceMemory = reinterpret_cast<BTTaskChaseMemory*>(NodeMemory);
	instanceMemory->ownerEnemy->GetAIController()->SetFocus(target);

	targetActor = target;
}
