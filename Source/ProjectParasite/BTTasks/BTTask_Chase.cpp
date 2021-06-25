// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Chase.h"

#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "ProjectParasite/AIControllers/ShooterAIController.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Pawns/PawnParasite.h"

UBTTask_Chase::UBTTask_Chase()
{
	NodeName = TEXT("Chase");

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BTTaskChaseMemory* instanceMemory = reinterpret_cast<BTTaskChaseMemory*>(NodeMemory);

	instanceMemory->blackboard = OwnerComp.GetBlackboardComponent();
	instanceMemory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();
	instanceMemory->shooterAIController = Cast<AShooterAIController>(instanceMemory->ownerEnemy->GetAIController());

	behaviorTreeComponent = &OwnerComp;
	playerRef = instanceMemory->ownerEnemy->GetPlayerRef();
	
	if(instanceMemory->shooterAIController == nullptr)
	{
		//Enemy executing this task isn't of a shooter type
		UE_LOG(LogTemp, Error, TEXT("Enemy %s executing this task isn't of a shooter type"), *instanceMemory->ownerEnemy->GetName())
		return EBTNodeResult::Failed;
	}

	//If player is possessing an enemy, we want this unit to chase the possessed enemy
	if(playerRef->GetPossessedEnemy() != nullptr)
	{
		SetTarget(playerRef->GetPossessedEnemy(), instanceMemory);
	}
	else
	{
		SetTarget(playerRef, instanceMemory);
	}
	
	instanceMemory->ownerEnemy->SetMoveSpeed(instanceMemory->ownerEnemy->GetChaseSpeed());
	return EBTNodeResult::InProgress;
}

void UBTTask_Chase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskChaseMemory* instanceMemory = reinterpret_cast<BTTaskChaseMemory*>(NodeMemory);

	if(instanceMemory->targetActor != nullptr)
	{
		//Chase the player
		instanceMemory->shooterAIController->MoveToActor(instanceMemory->targetActor, instanceMemory->ownerEnemy->GetAttackRange());

		//If reached the attack range, transition to attack state
		if(instanceMemory->shooterAIController->GetPathFollowingComponent()->DidMoveReachGoal())
		{
			instanceMemory->shooterAIController->SetCurrentState(ShooterStates::State_Attack);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No target actor"));
	}
}

void UBTTask_Chase::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{

	UE_LOG(LogTemp, Warning, TEXT("Exit Chase"));
	
	OwnerComp.GetAIOwner()->StopMovement();
	OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_Chase::SetTarget(APawnBase* target, BTTaskChaseMemory* instanceMemory)
{
	instanceMemory->ownerEnemy->GetAIController()->SetFocus(target);

	instanceMemory->targetActor = target;
}

BTTaskChaseMemory* UBTTask_Chase::GetInstanceMemory()
{
	uint8* nodeMemory = behaviorTreeComponent->GetNodeMemory(this, behaviorTreeComponent->GetActiveInstanceIdx());

	return reinterpret_cast<BTTaskChaseMemory*>(nodeMemory);
}

uint16 UBTTask_Chase::GetInstanceMemorySize() const
{
	return sizeof(BTTaskChaseMemory);
}