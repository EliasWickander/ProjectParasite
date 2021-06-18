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

	BTTaskPatrolBetweenWaypointsMemory* memory = reinterpret_cast<BTTaskPatrolBetweenWaypointsMemory*>(NodeMemory);

	memory->currentWaypoint = FVector::ZeroVector;
	memory->patrolPointQueue = new TQueue<FVector>();
	memory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();

	for(ATargetPoint* point : memory->ownerEnemy->GetPatrolPoints())
	{
		FVector tempLocation = point->GetActorLocation();
		tempLocation.Z = memory->ownerEnemy->GetActorLocation().Z;
		
		memory->patrolPointQueue->Enqueue(tempLocation);
	}

	InitNextWaypoint(NodeMemory);

	return EBTNodeResult::InProgress;
}

void UBTTask_PatrolBetweenWaypoints::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskPatrolBetweenWaypointsMemory* memory = reinterpret_cast<BTTaskPatrolBetweenWaypointsMemory*>(NodeMemory);
	
	if(memory->patrolPointQueue->IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no patrol points assigned. Cannot move."), *memory->ownerEnemy->GetName());
		return;	
	}

	memory->currentWaypoint.Z = memory->ownerEnemy->GetActorLocation().Z;

	if(FVector::Dist(memory->ownerEnemy->GetActorLocation(), memory->currentWaypoint) < 20)
	{
		memory->patrolPointQueue->Enqueue(memory->currentWaypoint);
		InitNextWaypoint(NodeMemory);
	}
}

void UBTTask_PatrolBetweenWaypoints::InitNextWaypoint(uint8* NodeMemory)
{
	BTTaskPatrolBetweenWaypointsMemory* memory = reinterpret_cast<BTTaskPatrolBetweenWaypointsMemory*>(NodeMemory);

	memory->patrolPointQueue->Dequeue(memory->currentWaypoint);

	memory->ownerEnemy->GetAIController()->SetFocalPoint(memory->currentWaypoint);
	memory->ownerEnemy->GetAIController()->MoveToLocation(memory->currentWaypoint, 1, false);
}

uint16 UBTTask_PatrolBetweenWaypoints::GetInstanceMemorySize() const
{
	return sizeof(BTTaskPatrolBetweenWaypointsMemory);
}

void UBTTask_PatrolBetweenWaypoints::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                    EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	BTTaskPatrolBetweenWaypointsMemory* memory = reinterpret_cast<BTTaskPatrolBetweenWaypointsMemory*>(NodeMemory);

	memory->ownerEnemy->GetAIController()->StopMovement();
}