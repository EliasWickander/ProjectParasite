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

	behaviorTreeComponent = &OwnerComp;
	
	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(NodeMemory);
	
	instanceMemory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();
	instanceMemory->playerRef = instanceMemory->ownerEnemy->GetPlayerRef();
	instanceMemory->blackboard = OwnerComp.GetBlackboardComponent();
	instanceMemory->shooterAIController = Cast<AShooterAIController>(OwnerComp.GetAIOwner());

	if(instanceMemory->shooterAIController == nullptr)
	{
		//Enemy executing this task isn't of a shooter type
		UE_LOG(LogTemp, Error, TEXT("Enemy %s executing this task isn't of a shooter type"), *instanceMemory->ownerEnemy->GetName())
		return EBTNodeResult::Failed;
	}

	//If player is possessing an enemy, we want this unit to attack the possessed enemy
	if(instanceMemory->playerRef->GetPossessedEnemy() != nullptr)
	{
		SetTarget(instanceMemory->playerRef->GetPossessedEnemy());
	}
	else
	{
		SetTarget(instanceMemory->playerRef);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(NodeMemory);

	//If target actor is dying, trigger event method
	if(instanceMemory->targetActor->GetIsPendingDeath())
	{
		OnTargetDeath(instanceMemory->targetActor);
	}

	//If enemy is in attack range, attack
	if(IsInRange(instanceMemory))
	{
		instanceMemory->ownerEnemy->Attack();
	}
	else
	{
		//If not, transition back to chase state
		instanceMemory->shooterAIController->SetCurrentState(ShooterStates::State_Chase);	
	}
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(NodeMemory);
}

uint16 UBTTask_Attack::GetInstanceMemorySize() const
{
	return sizeof(BTTaskAttackMemory);
}

void UBTTask_Attack::SetTarget(APawnBase* target)
{
	uint8* nodeMemory = behaviorTreeComponent->GetNodeMemory(this, behaviorTreeComponent->GetActiveInstanceIdx());

	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(nodeMemory);
	
	instanceMemory->ownerEnemy->GetAIController()->SetFocus(target);

	instanceMemory->targetActor = target;
}

void UBTTask_Attack::OnTargetDeath(AActor* destroyedActor)
{
	uint8* nodeMemory = behaviorTreeComponent->GetNodeMemory(this, behaviorTreeComponent->GetActiveInstanceIdx());

	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(nodeMemory);
	
	if(Cast<APawnEnemy>(destroyedActor))
	{
		SetTarget(instanceMemory->playerRef);
	}
}

bool UBTTask_Attack::IsInRange(BTTaskAttackMemory* instanceMemory)
{
	UPathFollowingComponent* pathFollowingComponent = instanceMemory->ownerEnemy->GetAIController()->GetPathFollowingComponent();

	return pathFollowingComponent->HasReached(*instanceMemory->targetActor, EPathFollowingReachMode::OverlapAgentAndGoal, instanceMemory->ownerEnemy->GetAttackRange(), true);;
}
