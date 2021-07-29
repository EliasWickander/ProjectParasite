// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Detect.h"

#include "AIController.h"
#include "ProjectParasite/Utilities/DevUtils.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectParasite/AIControllers/AIControllerBase.h"
#include "ApexDestruction/Public/DestructibleActor.h"

UBTTask_Detect::UBTTask_Detect()
{
	NodeName = TEXT("Detect");

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Detect::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BTTaskDetectMemory* instanceMemory = reinterpret_cast<BTTaskDetectMemory*>(NodeMemory);
	
	instanceMemory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();
	instanceMemory->blackboard = OwnerComp.GetBlackboardComponent();
	instanceMemory->enemyAIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	instanceMemory->hasDetected = false;
			
	if(instanceMemory->enemyAIController == nullptr)
	{
		//Enemy executing this task isn't of a shooter type
		UE_LOG(LogTemp, Error, TEXT("Enemy %s executing this task isn't of a shooter type"), *instanceMemory->ownerEnemy->GetName())
		return EBTNodeResult::Failed;
	}
	
	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Detect::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	BTTaskDetectMemory* instanceMemory = reinterpret_cast<BTTaskDetectMemory*>(NodeMemory);
	
	Detect(instanceMemory);

	if(instanceMemory->hasDetected)
	{
		if(instanceMemory->reactionTimer > instanceMemory->ownerEnemy->GetSightReactionTime())
		{
			instanceMemory->enemyAIController->SetCurrentState(EnemyStates::State_Chase);
		}
		else
		{
			instanceMemory->reactionTimer += GetWorld()->GetDeltaSeconds();
		}
	}
}

void UBTTask_Detect::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_Detect::Detect(BTTaskDetectMemory* instanceMemory)
{
	FBoxSphereBounds enemyColBounds = instanceMemory->ownerEnemy->GetCollider()->Bounds;

	//Define detection cone
	SCone coneData {
		enemyColBounds.Origin,
		instanceMemory->ownerEnemy->GetActorRotation().Quaternion(),
		instanceMemory->ownerEnemy->GetDetectionAngle(),
		enemyColBounds.BoxExtent.Z * 2,
		instanceMemory->ownerEnemy->GetDetectionRange()
	};

	const TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	const TArray<AActor*> actorsToIgnore { };
	UClass* classFilter = APawnParasite::StaticClass();
	TArray<AActor*> outActors;

	//If player is overlapping detection cone
	if(OverlapCone(coneData, GetWorld(), objectTypes, classFilter, actorsToIgnore, outActors))
	{
		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(instanceMemory->ownerEnemy);

		APawnParasite* detectedPlayer = Cast<APawnParasite>(outActors[0]);

		AActor* actorToDetect;

		//If player is possessing enemy, that enemy is the new focus of detection
		if(detectedPlayer->GetPossessedEnemy() != nullptr)
		{
			actorToDetect = detectedPlayer->GetPossessedEnemy();	
		}
		else
		{
			actorToDetect = detectedPlayer;
		}
		
		//Check if something is obstructing the vision of actor
		if(GetWorld()->LineTraceSingleByChannel(hitResult,instanceMemory->ownerEnemy->GetActorLocation(),actorToDetect->GetActorLocation(), ECC_Visibility, params))
		{
			if(hitResult.GetActor())
			{
				//If nothing is obstructing the vision
				if(hitResult.GetActor() == actorToDetect || Cast<ADestructibleActor>(hitResult.GetActor()))
				{
					instanceMemory->ownerEnemy->GetAIController()->StopMovement();
				
					instanceMemory->hasDetected = true;
				}	
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Didn't hit"));
		}
	}
}

uint16 UBTTask_Detect::GetInstanceMemorySize() const
{
	return sizeof(BTTaskDetectMemory);
}