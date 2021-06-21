// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Chase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"

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
	
	AAIController* AIController = OwnerComp.GetAIOwner();

	UObject* keyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
	AActor* targetActor = Cast<AActor>(keyValue);
	
	AIController->SetFocus(targetActor);

	memory->ownerEnemy->SetMoveSpeed(memory->ownerEnemy->GetChaseSpeed());
	return EBTNodeResult::InProgress;
}

void UBTTask_Chase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskChaseMemory* memory = reinterpret_cast<BTTaskChaseMemory*>(NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* keyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* targetActor = Cast<AActor>(keyValue);
		
		if(targetActor != nullptr)
		{
			AIController->MoveToActor(targetActor, memory->ownerEnemy->GetAttackRange());
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
