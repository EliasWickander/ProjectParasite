// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Actors/Weapons/WeaponBase.h"
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
	
	instanceMemory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();
	instanceMemory->blackboard = OwnerComp.GetBlackboardComponent();
	instanceMemory->shooterAIController = Cast<AShooterAIController>(OwnerComp.GetAIOwner());

	behaviorTreeComponent = &OwnerComp;
	playerRef = instanceMemory->ownerEnemy->GetPlayerRef();
	
	if(instanceMemory->shooterAIController == nullptr)
	{
		//Enemy executing this task isn't of a shooter type
		UE_LOG(LogTemp, Error, TEXT("Enemy %s executing this task isn't of a shooter type"), *instanceMemory->ownerEnemy->GetName())
		return EBTNodeResult::Failed;
	}

	//If player is possessing an enemy, we want this unit to attack the possessed enemy
	if(playerRef->GetPossessedEnemy() != nullptr)
	{
		SetTarget(playerRef->GetPossessedEnemy());
	}
	else
	{
		SetTarget(playerRef);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(NodeMemory);

	//If target actor is dead, trigger event method
	if(instanceMemory->targetActor->GetIsPendingDeath())
	{
		OnTargetDeath(instanceMemory->targetActor);
	}
	
	//If enemy is in attack range, rotate weapon and attack
	if(IsInRange(NodeMemory))
	{
		RotateWeaponToTarget();
		
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
}

void UBTTask_Attack::SetTarget(APawnBase* target)
{
	BTTaskAttackMemory* instanceMemory = GetInstanceMemory();
	
	instanceMemory->ownerEnemy->GetAIController()->SetFocus(target);

	instanceMemory->targetActor = target;
}

void UBTTask_Attack::OnTargetDeath(AActor* deadActor)
{
	//If dead target was enemy, set parasite as new target
	if(Cast<APawnEnemy>(deadActor))
	{
		SetTarget(playerRef);
	}
}

bool UBTTask_Attack::IsInRange(uint8* nodeMemory)
{
	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(nodeMemory);
	
	UPathFollowingComponent* pathFollowingComponent = instanceMemory->ownerEnemy->GetAIController()->GetPathFollowingComponent();

	return pathFollowingComponent->HasReached(*instanceMemory->targetActor, EPathFollowingReachMode::OverlapAgentAndGoal, instanceMemory->ownerEnemy->GetAttackRange(), true);;
}

void UBTTask_Attack::RotateWeaponToTarget()
{
	//Rotates weapon towards target (will later be replaced by joint rotation logic)
	
	BTTaskAttackMemory* instanceMemory = GetInstanceMemory();

	AWeaponBase* weapon = instanceMemory->ownerEnemy->GetWeapon();
	
	FVector dirToTarget = instanceMemory->targetActor->GetActorLocation() - weapon->GetActorLocation();
		
	weapon->SetActorRotation(dirToTarget.Rotation());
}

BTTaskAttackMemory* UBTTask_Attack::GetInstanceMemory()
{
	uint8* nodeMemory = behaviorTreeComponent->GetNodeMemory(this, behaviorTreeComponent->GetActiveInstanceIdx());

	return reinterpret_cast<BTTaskAttackMemory*>(nodeMemory);
}

uint16 UBTTask_Attack::GetInstanceMemorySize() const
{
	return sizeof(BTTaskAttackMemory);
}