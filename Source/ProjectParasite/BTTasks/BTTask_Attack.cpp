// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/AIControllers/ShooterAIController.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Pawns/PawnParasite.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(NodeMemory);
	
	ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();
	playerRef = ownerEnemy->GetPlayerRef();
	blackboard = OwnerComp.GetBlackboardComponent();
	
	if(playerRef->GetPossessedEnemy() != nullptr)
	{
		SetTarget(playerRef->GetPossessedEnemy());
	}
	else
	{
		SetTarget(playerRef);
	}

	targetActor->OnDestroyed.AddDynamic(this, &UBTTask_Attack::OnTargetDestroyed);
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(NodeMemory);

	UPathFollowingComponent* pathFollowingComponent = ownerEnemy->GetAIController()->GetPathFollowingComponent();

	bool isInRange = pathFollowingComponent->HasReached(*targetActor, EPathFollowingReachMode::OverlapAgentAndGoal, ownerEnemy->GetAttackRange(), true);

	if(isInRange)
	{
		ownerEnemy->Attack();
	}
	else
	{
		blackboard->SetValueAsEnum("CurrentState", (uint8)ShooterStates::State_Chase);
	}
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

uint16 UBTTask_Attack::GetInstanceMemorySize() const
{
	return sizeof(BTTaskAttackMemory);
}

void UBTTask_Attack::SetTarget(AActor* target)
{
	ownerEnemy->GetAIController()->SetFocus(target);

	targetActor = target;
}

void UBTTask_Attack::OnTargetDestroyed(AActor* destroyedActor)
{
	if(Cast<APawnEnemy>(destroyedActor))
	{
		SetTarget(playerRef);
	}
}
