// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PatrolBetweenWaypoints.h"

#include "ProjectParasite/AIControllers/AIControllerBase.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "Engine/TargetPoint.h"
#include "ProjectParasite/Utilities/DevUtils.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"

UBTTask_PatrolBetweenWaypoints::UBTTask_PatrolBetweenWaypoints()
{
	NodeName = TEXT("Patrol Between Waypoints");

	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_PatrolBetweenWaypoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BTTaskPatrolBetweenWaypointsMemory* instanceMemory = reinterpret_cast<BTTaskPatrolBetweenWaypointsMemory*>(NodeMemory);

	//if this is the first time this task is executed (to save current waypoint if patrolling is interrupted)
	if(instanceMemory->ownerEnemy == nullptr)
	{
		instanceMemory->currentWaypoint = FVector::ZeroVector;
		instanceMemory->patrolPointQueue = new TQueue<FVector>();
		instanceMemory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();	
	} 

	instanceMemory->ownerEnemy->SetMoveSpeed(instanceMemory->ownerEnemy->GetPatrolSpeed());

	instanceMemory->finishedRotating = false;
	
	SetupPatrolPoints(instanceMemory);
	InitNextWaypoint(instanceMemory);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_PatrolBetweenWaypoints::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskPatrolBetweenWaypointsMemory* instanceMemory = reinterpret_cast<BTTaskPatrolBetweenWaypointsMemory*>(NodeMemory);

	APawnEnemy* ownerEnemy = instanceMemory->ownerEnemy;
	if(instanceMemory->patrolPointQueue->IsEmpty())
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s has no patrol points assigned. Cannot move."), *instanceMemory->ownerEnemy->GetName());
		return;	
	}
	
	AAIControllerBase* AIController = instanceMemory->ownerEnemy->GetAIController();

	if(!instanceMemory->finishedRotating)
	{
		if(SetFocalPointExtended(AIController, instanceMemory->currentWaypoint, ownerEnemy->GetTurnRate(), 0.2f))
		{
			instanceMemory->finishedRotating = true;
		}
	}

	if(instanceMemory->finishedRotating)
	{
		AIController->MoveToLocation(instanceMemory->currentWaypoint, 0, false);
		
		SetFocalPointExtended(AIController, instanceMemory->ownerEnemy->GetVelocity(), ownerEnemy->GetTurnRate(), 0.2f);
		instanceMemory->reachedGoalLastFrame = false;
	}

	instanceMemory->currentWaypoint.Z = instanceMemory->ownerEnemy->GetActorLocation().Z;
	FVector dirToTargetPoint = instanceMemory->currentWaypoint - instanceMemory->ownerEnemy->GetActorLocation();
	
	float distToTargetPoint = dirToTargetPoint.Size();
	
	//If enemy reached current waypoint, initialize next one in queue
	if(distToTargetPoint <= 50 && !instanceMemory->reachedGoalLastFrame)
	{
		instanceMemory->patrolPointQueue->Enqueue(instanceMemory->currentWaypoint);
		InitNextWaypoint(instanceMemory);
		instanceMemory->finishedRotating = false;
		instanceMemory->reachedGoalLastFrame = true;
	}
}

void UBTTask_PatrolBetweenWaypoints::InitNextWaypoint(BTTaskPatrolBetweenWaypointsMemory* instanceMemory)
{
	AAIControllerBase* AIController = instanceMemory->ownerEnemy->GetAIController();

	//Set new current waypoint and move to it
	instanceMemory->patrolPointQueue->Dequeue(instanceMemory->currentWaypoint);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *instanceMemory->currentWaypoint.ToString());
}

void UBTTask_PatrolBetweenWaypoints::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                    EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	BTTaskPatrolBetweenWaypointsMemory* instanceMemory = reinterpret_cast<BTTaskPatrolBetweenWaypointsMemory*>(NodeMemory);

	instanceMemory->ownerEnemy->GetAIController()->StopMovement();
}

void UBTTask_PatrolBetweenWaypoints::SetupPatrolPoints(BTTaskPatrolBetweenWaypointsMemory* instanceMemory)
{
	//Set up queue for patrol points
	for(ATargetPoint* point : instanceMemory->ownerEnemy->GetPatrolPoints())
	{
		FVector tempLocation = point->GetActorLocation();
		tempLocation.Z = instanceMemory->ownerEnemy->GetActorLocation().Z;
		
		instanceMemory->patrolPointQueue->Enqueue(tempLocation);
	}
}

uint16 UBTTask_PatrolBetweenWaypoints::GetInstanceMemorySize() const
{
	return sizeof(BTTaskPatrolBetweenWaypointsMemory);
}
