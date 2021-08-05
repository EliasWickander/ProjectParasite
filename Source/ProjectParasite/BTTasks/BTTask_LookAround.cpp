// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LookAround.h"

#include "AIController.h"
#include "ProjectParasite/AIControllers/AIControllerBase.h"
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

	instanceMemory->stateTimer = instanceMemory->ownerEnemy->GetLookAroundTime();
	instanceMemory->startRot = instanceMemory->ownerEnemy->GetActorRotation();
	instanceMemory->currentRot = instanceMemory->startRot;
	instanceMemory->lookAroundTimer = 0;

	instanceMemory->targetAngle = 60;
	instanceMemory->targetRot = instanceMemory->startRot.Vector().RotateAngleAxis(instanceMemory->targetAngle, FVector::UpVector).Rotation();

	instanceMemory->finishedMoving = false;

	instanceMemory->ownerEnemy->GetAIController()->StopMovement();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_LookAround::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	BTTaskLookAroundMemory* instanceMemory = reinterpret_cast<BTTaskLookAroundMemory*>(NodeMemory);

	if(instanceMemory->stateTimer > 0)
	{
		instanceMemory->stateTimer -= DeltaSeconds;
		Search(NodeMemory);
	}
	else
	{
		instanceMemory->ownerEnemy->GetAIController()->SetCurrentState(EnemyStates::State_Patrol);
	}
}

void UBTTask_LookAround::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	BTTaskLookAroundMemory* instanceMemory = reinterpret_cast<BTTaskLookAroundMemory*>(NodeMemory);
}

void UBTTask_LookAround::Search(uint8* NodeMemory)
{
	BTTaskLookAroundMemory* instanceMemory = reinterpret_cast<BTTaskLookAroundMemory*>(NodeMemory);
	
	float lookAroundTime = instanceMemory->ownerEnemy->GetLookAroundTime() * 0.5f;

	LookAtRotation(lookAroundTime, NodeMemory);
}

bool UBTTask_LookAround::LookAtRotation(float duration, uint8* NodeMemory)
{
	BTTaskLookAroundMemory* instanceMemory = reinterpret_cast<BTTaskLookAroundMemory*>(NodeMemory);

	if(instanceMemory->lookAroundTimer < duration)
	{
		instanceMemory->lookAroundTimer += GetWorld()->GetDeltaSeconds();

		FRotator lerpedRot = FMath::Lerp(instanceMemory->currentRot, instanceMemory->targetRot, instanceMemory->lookAroundTimer / duration);

		instanceMemory->ownerEnemy->SetActorRotation(lerpedRot);
		return false;
	}
	else
	{
		instanceMemory->lookAroundTimer = 0;
		instanceMemory->targetAngle = -instanceMemory->targetAngle;
		instanceMemory->targetRot = instanceMemory->startRot.Vector().RotateAngleAxis(instanceMemory->targetAngle, FVector::UpVector).Rotation();
		
		instanceMemory->currentRot = instanceMemory->ownerEnemy->GetActorRotation();
		return true;
	}
}

uint16 UBTTask_LookAround::GetInstanceMemorySize() const
{
	return sizeof(BTTaskLookAroundMemory);
}
