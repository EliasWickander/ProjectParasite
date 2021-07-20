// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Guard.h"

#include "ProjectParasite/AIControllers/AIControllerBase.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "Engine/TargetPoint.h"
#include "ProjectParasite/Utilities/DevUtils.h"
#include "DrawDebugHelpers.h"
UBTTask_Guard::UBTTask_Guard()
{
	NodeName = TEXT("Guard");

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Guard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BTTaskGuardMemory* instanceMemory = reinterpret_cast<BTTaskGuardMemory*>(NodeMemory);

	instanceMemory->currentGuardPoint = FVector::ZeroVector;
	instanceMemory->guardPointQueue = new TQueue<FVector>();
	instanceMemory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();

	instanceMemory->ownerEnemy->SetMoveSpeed(instanceMemory->ownerEnemy->GetPatrolSpeed());

	SetupGuardPoints(instanceMemory);
	InitNextGuardPoint(instanceMemory);

	return EBTNodeResult::InProgress;
}

void UBTTask_Guard::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskGuardMemory* instanceMemory = reinterpret_cast<BTTaskGuardMemory*>(NodeMemory);

	APawnEnemy* ownerEnemy = instanceMemory->ownerEnemy;
	if(instanceMemory->guardPointQueue->IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no guard points assigned."), *instanceMemory->ownerEnemy->GetName());
		return;	
	}
	
	AAIControllerBase* AIController = instanceMemory->ownerEnemy->GetAIController();
	
	bool finishedRotating = SetFocalPointExtended(AIController, instanceMemory->currentGuardPoint, ownerEnemy->GetTurnRate(), 0.2f);

	if(finishedRotating)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"), guardTimer);
		if(guardTimer <= 0)
		{
			instanceMemory->guardPointQueue->Enqueue(instanceMemory->currentGuardPoint);
			InitNextGuardPoint(instanceMemory);
		}
		else
		{
			guardTimer -= DeltaSeconds;
		}
	}
}

void UBTTask_Guard::InitNextGuardPoint(BTTaskGuardMemory* instanceMemory)
{
	AAIControllerBase* AIController = instanceMemory->ownerEnemy->GetAIController();

	//Set new current waypoint and move to it
	instanceMemory->guardPointQueue->Dequeue(instanceMemory->currentGuardPoint);

	float guardTime = instanceMemory->ownerEnemy->GetGuardTime();
	float offset = instanceMemory->ownerEnemy->GetGuardTimeOffset();
	
	guardTimer = FMath::RandRange(guardTime - offset, guardTime + offset);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *instanceMemory->currentWaypoint.ToString());
}

void UBTTask_Guard::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                    EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	BTTaskGuardMemory* instanceMemory = reinterpret_cast<BTTaskGuardMemory*>(NodeMemory);

	instanceMemory->ownerEnemy->GetAIController()->StopMovement();
}

void UBTTask_Guard::SetupGuardPoints(BTTaskGuardMemory* instanceMemory)
{
	//Set up queue for patrol points
	for(ATargetPoint* point : instanceMemory->ownerEnemy->GetGuardPoints())
	{
		FVector tempLocation = point->GetActorLocation();
		tempLocation.Z = instanceMemory->ownerEnemy->GetActorLocation().Z;
		
		instanceMemory->guardPointQueue->Enqueue(tempLocation);
	}
}

uint16 UBTTask_Guard::GetInstanceMemorySize() const
{
	return sizeof(BTTaskGuardMemory);
}
