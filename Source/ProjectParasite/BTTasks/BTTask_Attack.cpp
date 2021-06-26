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
		SetTarget(playerRef->GetPossessedEnemy(), NodeMemory);
	}
	else
	{
		SetTarget(playerRef, NodeMemory);
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
		OnTargetDeath(instanceMemory->targetActor, NodeMemory);
	}

	bool rotatedToTarget = RotateBodyToTarget(instanceMemory->ownerEnemy->GetTurnRate(), 0.2f, NodeMemory);
	
	//If enemy is in attack range, rotate weapon and attack
	if(IsInRange(NodeMemory))
	{
		if(rotatedToTarget)
		{
			RotateWeaponToTarget(NodeMemory);	
			instanceMemory->ownerEnemy->Attack();
		}
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

	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(NodeMemory);
	
	AWeaponBase* weapon = instanceMemory->ownerEnemy->GetWeapon();

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

bool UBTTask_Attack::RotateBodyToTarget(float turnRate, float acceptanceDist, uint8* nodeMemory)
{
	BTTaskAttackMemory* instanceMemory = reinterpret_cast<BTTaskAttackMemory*>(nodeMemory);
	
	APawnEnemy* ownerEnemy = instanceMemory->ownerEnemy;
	APawnBase* targetActor = instanceMemory->targetActor;
	
	FVector dirToTarget = targetActor->GetActorLocation() - ownerEnemy->GetActorLocation();
	dirToTarget.Normalize();

	//Interpolate between owners forward dir and target dir
	FVector finalFocalPoint = FMath::Lerp(ownerEnemy->GetActorForwardVector(), dirToTarget, turnRate * GetWorld()->GetDeltaSeconds());

	//Set ai to look in final direction
	ownerEnemy->GetAIController()->SetFocalPoint(ownerEnemy->GetActorLocation() + finalFocalPoint);

	//Returns true if distance between final focal point and target dir is less than acceptance dist
	return FVector::Dist(finalFocalPoint, dirToTarget) <= acceptanceDist;
	
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
