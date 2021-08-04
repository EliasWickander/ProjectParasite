// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Actors/Weapons/WeaponBase.h"
#include "ProjectParasite/AIControllers/AIControllerBase.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/RangedPawnEnemy.h"
#include "ProjectParasite/PlayerControllers/PlayerControllerBase.h"
#include "ProjectParasite/Utilities/DevUtils.h"
#include "ProjectParasite/Actors/Weapons/RangedWeapon.h"

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

	instanceMemory->ownerEnemy->SetMoveSpeed(instanceMemory->ownerEnemy->GetAttackMoveSpeed());

	instanceMemory->attackTimer = 0;
	instanceMemory->preparingAttack = false;
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

	if(Cast<ARangedPawnEnemy>(instanceMemory->ownerEnemy))
	{
		FVector dirToTarget = targetActor->GetActorLocation() - ownerEnemy->GetActorLocation();
		dirToTarget.Z = 0;

		if(dirToTarget.Size() < ownerEnemy->GetAttackRange())
		{
			bool finishedRetreating = Retreat(NodeMemory);

			if(!finishedRetreating)
				return;
		}
	}

	bool rotatedToTarget = SetFocusExtended(ownerEnemy->GetAIController(), targetActor, ownerEnemy->GetTurnRate(), 0.02f);

	if(rotatedToTarget)
	{
		RotateWeaponToTarget(NodeMemory);	
		ownerEnemy->Attack();
	}
	
	//If enemy is not in attack range, transition back to chase state
	if(!IsInRange(NodeMemory))
	{
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

	instanceMemory->targetActor = target;
}

void UBTTask_Attack::OnTargetDeath(AActor* deadActor, uint8* nodeMemory)
{
	//If dead target was enemy, set parasite as new target
	if(Cast<APawnEnemy>(deadActor))
	{
		SetTarget(playerRef->playerControllerRef->GetPlayer(), nodeMemory);
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
	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(nodeMemory);

	AWeaponBase* weapon = instanceMemory->ownerEnemy->GetWeapon();

	FVector dirToTarget = instanceMemory->targetActor->GetCollider()->GetComponentLocation() - weapon->GetActorLocation();

	weapon->SetActorRotation(dirToTarget.Rotation());
}

bool UBTTask_Attack::Retreat(uint8* nodeMemory)
{
	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(nodeMemory);

	APawnEnemy* ownerEnemy = instanceMemory->ownerEnemy;
	APawnBase* targetActor = instanceMemory->targetActor;

	//Check if there is obstacle behind enemy
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByObjectType(
		hitResult,
		ownerEnemy->GetActorLocation(),
		ownerEnemy->GetActorLocation() - ownerEnemy->GetActorForwardVector() * 200,
		ECollisionChannel::ECC_WorldStatic);

	if(hitResult.GetActor() == nullptr)
	{
		if(instanceMemory->backOffTimer < 0.2f)
		{
			instanceMemory->backOffTimer += GetWorld()->GetDeltaSeconds();
		}
		else
		{
			FVector safePoint = ownerEnemy->GetActorLocation() - ownerEnemy->GetActorForwardVector() * ownerEnemy->GetAttackRange();
			instanceMemory->backOffTimer = 0;

			ownerEnemy->GetAIController()->MoveToLocation(safePoint);
			ownerEnemy->GetAIController()->SetFocus(targetActor);
		}
	}
	else
	{
		return true;
	}

	return false;
}

uint16 UBTTask_Attack::GetInstanceMemorySize() const
{
	return sizeof(BTTaskAttackMemory);
}
