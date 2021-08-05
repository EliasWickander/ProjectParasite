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

	SetupPatrolPoints(instanceMemory);
	InitNextWaypoint(instanceMemory);

	UE_LOG(LogTemp, Warning, TEXT("%s start patrolling"), *instanceMemory->ownerEnemy->GetName());
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
	
	bool finishedRotating = SetFocalPointExtended(AIController, instanceMemory->currentWaypoint, ownerEnemy->GetTurnRate(), 0.2f);

	if(finishedRotating)
	{
		AIController->MoveToLocation(instanceMemory->currentWaypoint, 20, false);
		instanceMemory->reachedGoalLastFrame = false;
	}
	
	//If enemy reached current waypoint, initialize next one in queue
	if(AIController->GetPathFollowingComponent()->DidMoveReachGoal() && !instanceMemory->reachedGoalLastFrame)
	{
		instanceMemory->patrolPointQueue->Enqueue(instanceMemory->currentWaypoint);
		InitNextWaypoint(instanceMemory);
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

	UE_LOG(LogTemp, Warning, TEXT("Exit patrol"));
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
