// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PatrolBetweenWaypoints.h"

#include "AIController.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "Engine/TargetPoint.h"

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

	instanceMemory->currentWaypoint = FVector::ZeroVector;
	instanceMemory->patrolPointQueue = new TQueue<FVector>();
	instanceMemory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();

	instanceMemory->ownerEnemy->SetMoveSpeed(instanceMemory->ownerEnemy->GetPatrolSpeed());

	SetupPatrolPoints(NodeMemory);
	InitNextWaypoint(NodeMemory);

	return EBTNodeResult::InProgress;
}

void UBTTask_PatrolBetweenWaypoints::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskPatrolBetweenWaypointsMemory* instanceMemory = reinterpret_cast<BTTaskPatrolBetweenWaypointsMemory*>(NodeMemory);
	
	if(instanceMemory->patrolPointQueue->IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no patrol points assigned. Cannot move."), *instanceMemory->ownerEnemy->GetName());
		return;	
	}

	AAIController* AIController = instanceMemory->ownerEnemy->GetAIController();

	//If enemy reached current waypoint, initialize next one in queue
	if(AIController->GetPathFollowingComponent()->DidMoveReachGoal())
	{
		instanceMemory->patrolPointQueue->Enqueue(instanceMemory->currentWaypoint);
		InitNextWaypoint(NodeMemory);
	}
}

void UBTTask_PatrolBetweenWaypoints::InitNextWaypoint(uint8* nodeMemory)
{
	BTTaskPatrolBetweenWaypointsMemory* instanceMemory = reinterpret_cast<BTTaskPatrolBetweenWaypointsMemory*>(nodeMemory);

	instanceMemory->patrolPointQueue->Dequeue(instanceMemory->currentWaypoint);

	instanceMemory->ownerEnemy->GetAIController()->SetFocalPoint(instanceMemory->currentWaypoint);
	instanceMemory->ownerEnemy->GetAIController()->MoveToLocation(instanceMemory->currentWaypoint, 20, false);
}

void UBTTask_PatrolBetweenWaypoints::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                    EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	BTTaskPatrolBetweenWaypointsMemory* instanceMemory = reinterpret_cast<BTTaskPatrolBetweenWaypointsMemory*>(NodeMemory);

	instanceMemory->ownerEnemy->GetAIController()->StopMovement();
}

void UBTTask_PatrolBetweenWaypoints::SetupPatrolPoints(uint8* nodeMemory)
{
	BTTaskPatrolBetweenWaypointsMemory* instanceMemory = reinterpret_cast<BTTaskPatrolBetweenWaypointsMemory*>(nodeMemory);

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
