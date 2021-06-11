// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PatrolBetweenWaypoints.h"

#include "AIController.h"
#include "Pawns/PawnEnemy.h"
#include "Engine/TargetPoint.h"

UBTTask_PatrolBetweenWaypoints::UBTTask_PatrolBetweenWaypoints()
{
	NodeName = TEXT("Patrol Between Waypoints");

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PatrolBetweenWaypoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();

	for(ATargetPoint* point : ownerEnemy->GetPatrolPoints())
	{
		FVector tempLocation = point->GetActorLocation();
		tempLocation.Z = ownerEnemy->GetActorLocation().Z;
		
		patrolPointQueue.Enqueue(tempLocation);
	}

	InitNextWaypoint();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_PatrolBetweenWaypoints::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(patrolPointQueue.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has no patrol points assigned. Cannot move."), *ownerEnemy->GetName());
		return;	
	}

	currentWaypoint.Z = ownerEnemy->GetActorLocation().Z;

	if(FVector::Dist(ownerEnemy->GetActorLocation(), currentWaypoint) < 20)
	{
		patrolPointQueue.Enqueue(currentWaypoint);
		InitNextWaypoint();
	}
}

void UBTTask_PatrolBetweenWaypoints::InitNextWaypoint()
{
	patrolPointQueue.Dequeue(currentWaypoint);

	ownerEnemy->GetAIController()->SetFocalPoint(currentWaypoint);
	ownerEnemy->GetAIController()->MoveToLocation(currentWaypoint, 1, false);
}

void UBTTask_PatrolBetweenWaypoints::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
