// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Chase.h"

#include "AIController.h"
#include "DestructibleActor.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/AIControllers/AIControllerBase.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Utilities/DevUtils.h"

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
	instanceMemory->enemyAIController = Cast<AAIControllerBase>(instanceMemory->ownerEnemy->GetAIController());

	behaviorTreeComponent = &OwnerComp;
	playerRef = Cast<APawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if(instanceMemory->enemyAIController == nullptr)
	{
		//Enemy executing this task isn't of a shooter type
		UE_LOG(LogTemp, Error, TEXT("Enemy %s executing this task isn't of a shooter type"), *instanceMemory->ownerEnemy->GetName())
		return EBTNodeResult::Failed;
	}
	
	//Chase the player
	SetTarget(playerRef, instanceMemory);
	
	instanceMemory->ownerEnemy->SetMoveSpeed(instanceMemory->ownerEnemy->GetChaseSpeed());
	instanceMemory->obstructed = false;

	instanceMemory->ownerEnemy->OnChaseStart();
	return EBTNodeResult::InProgress;
}

void UBTTask_Chase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskChaseMemory* instanceMemory = reinterpret_cast<BTTaskChaseMemory*>(NodeMemory);

	if(instanceMemory->targetActor != nullptr)
	{
		SetFocusExtended(instanceMemory->enemyAIController, instanceMemory->targetActor, instanceMemory->ownerEnemy->GetTurnRate(), 100);

		if(instanceMemory->ownerEnemy->IsTargetObstructed(instanceMemory->targetActor))
		{
			if(instanceMemory->obstructed == false)
			{
				instanceMemory->obstructed = true;

				instanceMemory->ownerEnemy->SetLastSeenPos(instanceMemory->targetActor->GetActorLocation());
				instanceMemory->ownerEnemy->GetAIController()->MoveToLocation(instanceMemory->ownerEnemy->GetLastSeenPos(), 0);
			}

			if(FVector::Dist(instanceMemory->ownerEnemy->GetActorLocation(), instanceMemory->ownerEnemy->GetLastSeenPos()) < 100 || instanceMemory->ownerEnemy->GetAIController()->GetMoveStatus() == EPathFollowingStatus::Idle)
			{
				instanceMemory->ownerEnemy->GetAIController()->StopMovement();
				
				if(instanceMemory->lookAroundTransitionTimer < instanceMemory->ownerEnemy->GetChaseToLookAroundTransitionTime())
				{
					instanceMemory->lookAroundTransitionTimer += DeltaSeconds;
				}
				else
				{
					instanceMemory->lookAroundTransitionTimer = 0;
					instanceMemory->enemyAIController->SetCurrentState(EnemyStates::State_LookAround);	
				}	
			}
		}
		else
		{
			instanceMemory->lookAroundTransitionTimer = 0;
			instanceMemory->obstructed = false;

			//Chase the player
			instanceMemory->enemyAIController->MoveToActor(instanceMemory->targetActor, instanceMemory->ownerEnemy->GetAttackRangeMin());

			float distToTarget = FVector::Dist(instanceMemory->ownerEnemy->GetActorLocation(), instanceMemory->targetActor->GetActorLocation());
		
			//If reached the attack range, transition to attack state
			if(distToTarget < instanceMemory->ownerEnemy->GetAttackRangeMax())
			{
				instanceMemory->enemyAIController->SetCurrentState(EnemyStates::State_Attack);
			}
		}	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No target actor"));
	}
}

void UBTTask_Chase::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	
	OwnerComp.GetAIOwner()->StopMovement();
	OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_Chase::SetTarget(APawnBase* target, BTTaskChaseMemory* instanceMemory)
{
	//instanceMemory->ownerEnemy->GetAIController()->SetFocus(target);

	instanceMemory->targetActor = target;
}

uint16 UBTTask_Chase::GetInstanceMemorySize() const
{
	return sizeof(BTTaskChaseMemory);
}