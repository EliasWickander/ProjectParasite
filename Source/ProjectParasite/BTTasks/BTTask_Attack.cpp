// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Actors/Weapons/WeaponBase.h"
#include "ProjectParasite/AIControllers/AIControllerBase.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Utilities/DevUtils.h"

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
	instanceMemory->enemyAIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());

	behaviorTreeComponent = &OwnerComp;

	playerRef = Cast<APawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if(instanceMemory->enemyAIController == nullptr)
	{
		//Enemy executing this task isn't of a shooter type
		UE_LOG(LogTemp, Error, TEXT("Enemy %s executing this task isn't of a shooter type"), *instanceMemory->ownerEnemy->GetName())
		return EBTNodeResult::Failed;
	}

	SetTarget(playerRef, NodeMemory);

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(NodeMemory);

	APawnEnemy* ownerEnemy = instanceMemory->ownerEnemy;
	APawnBase* targetActor = instanceMemory->targetActor;
	
	//If target actor is dead, trigger event method
	if(targetActor->GetIsPendingDeath())
	{
		OnTargetDeath(targetActor, NodeMemory);
	}

	bool rotatedToTarget = SetFocusExtended(ownerEnemy->GetAIController(), targetActor, ownerEnemy->GetTurnRate(), 0.2f);
	
	//If enemy is in attack range, rotate weapon and attack
	if(IsInRange(NodeMemory))
	{
		if(rotatedToTarget)
		{
			RotateWeaponToTarget(NodeMemory);	
			ownerEnemy->Attack();
		}
	}
	else
	{
		//If not, transition back to chase state
		instanceMemory->enemyAIController->SetCurrentState(EnemyStates::State_Chase);	
	}
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(NodeMemory);
	
	AWeaponBase* weapon = instanceMemory->ownerEnemy->GetWeapon();

	if(weapon)
		weapon->SetActorRotation(instanceMemory->ownerEnemy->GetActorForwardVector().Rotation());
	
}

void UBTTask_Attack::SetTarget(APawnBase* target, uint8* nodeMemory)
{
	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(nodeMemory);
	
	//instanceMemory->ownerEnemy->GetAIController()->SetFocus(target);

	instanceMemory->targetActor = target;
}

void UBTTask_Attack::OnTargetDeath(AActor* deadActor, uint8* nodeMemory)
{
	//If dead target was enemy, set parasite as new target
	if(Cast<APawnEnemy>(deadActor))
	{
		SetTarget(playerRef, nodeMemory);
	}
}

bool UBTTask_Attack::IsInRange(uint8* nodeMemory)
{
	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(nodeMemory);
	
	UPathFollowingComponent* pathFollowingComponent = instanceMemory->ownerEnemy->GetAIController()->GetPathFollowingComponent();

	return pathFollowingComponent->HasReached(*instanceMemory->targetActor, EPathFollowingReachMode::OverlapAgentAndGoal, instanceMemory->ownerEnemy->GetAttackRange(), true);;
}

void UBTTask_Attack::RotateWeaponToTarget(uint8* nodeMemory)
{
	//Rotates weapon towards target (will later be replaced by joint rotation logic)

	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(nodeMemory);

	AWeaponBase* weapon = instanceMemory->ownerEnemy->GetWeapon();
	
	FVector dirToTarget = instanceMemory->targetActor->GetActorLocation() - weapon->GetActorLocation();

	weapon->SetActorRotation(dirToTarget.Rotation());
}

uint16 UBTTask_Attack::GetInstanceMemorySize() const
{
	return sizeof(BTTaskAttackMemory);
}
