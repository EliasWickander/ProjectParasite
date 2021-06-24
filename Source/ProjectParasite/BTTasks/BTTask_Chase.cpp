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

	//Only accept actor blackboard key
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Chase, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BTTaskChaseMemory* instanceMemory = reinterpret_cast<BTTaskChaseMemory*>(NodeMemory);
	
	instanceMemory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();

	instanceMemory->blackboard = OwnerComp.GetBlackboardComponent();

	playerRef = instanceMemory->ownerEnemy->GetPlayerRef();
	instanceMemory->shooterAIController = Cast<AShooterAIController>(instanceMemory->ownerEnemy->GetAIController());

	if(instanceMemory->shooterAIController == nullptr)
	{
		//Enemy executing this task isn't of a shooter type
		UE_LOG(LogTemp, Error, TEXT("Enemy %s executing this task isn't of a shooter type"), *instanceMemory->ownerEnemy->GetName())
		return EBTNodeResult::Failed;
	}

	if(playerRef->GetPossessedEnemy() != nullptr)
	{
		SetTarget(NodeMemory, playerRef->GetPossessedEnemy());
	}
	else
	{
		SetTarget(NodeMemory, playerRef);
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
		instanceMemory->shooterAIController->MoveToActor(instanceMemory->targetActor, instanceMemory->ownerEnemy->GetAttackRange());

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

uint16 UBTTask_Chase::GetInstanceMemorySize() const
{
	return sizeof(BTTaskChaseMemory);
}

void UBTTask_Chase::SetTarget(uint8* NodeMemory, AActor* target)
{
	BTTaskChaseMemory* instanceMemory = reinterpret_cast<BTTaskChaseMemory*>(NodeMemory);
	instanceMemory->ownerEnemy->GetAIController()->SetFocus(target);

	instanceMemory->targetActor = target;
}
