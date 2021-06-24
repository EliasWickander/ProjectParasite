// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Detect.h"

#include "AIController.h"
#include "ProjectParasite/Utilities/DevUtils.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectParasite/AIControllers/ShooterAIController.h"

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
	instanceMemory->shooterAIController = Cast<AShooterAIController>(OwnerComp.GetAIOwner());

	if(instanceMemory->shooterAIController == nullptr)
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
	
	Detect(NodeMemory);

	if(hasDetected)
	{
		if(reactionTimer > instanceMemory->ownerEnemy->GetSightReactionTime())
		{
			hasDetected = false;
			
			instanceMemory->shooterAIController->SetCurrentState(ShooterStates::State_Chase);
		}
		else
		{
			reactionTimer += GetWorld()->GetDeltaSeconds();
		}
	}
}

void UBTTask_Detect::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_Detect::Detect(uint8* nodeMemory)
{
	BTTaskDetectMemory* memory = reinterpret_cast<BTTaskDetectMemory*>(nodeMemory);
	
	FBoxSphereBounds enemyColBounds = memory->ownerEnemy->GetCollider()->Bounds;

	SCone coneData {
		enemyColBounds.Origin,
		memory->ownerEnemy->GetActorRotation().Quaternion(),
		memory->ownerEnemy->GetDetectionAngle(),
		enemyColBounds.BoxExtent.Z * 2,
		memory->ownerEnemy->GetDetectionRange()
	};

	const TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	const TArray<AActor*> actorsToIgnore { };
	UClass* classFilter = APawnParasite::StaticClass();
	TArray<AActor*> outActors;
	
	if(OverlapCone(coneData, GetWorld(), objectTypes, classFilter, actorsToIgnore, outActors))
	{
		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(memory->ownerEnemy);

		if(GetWorld()->LineTraceSingleByChannel(hitResult,memory->ownerEnemy->GetActorLocation(),outActors[0]->GetActorLocation(), ECC_Visibility, params))
		{
			AActor* actorToDetect;

			if(playerRef->GetPossessedEnemy() != nullptr)
			{
				actorToDetect = playerRef->GetPossessedEnemy();	
			}
			else
			{
				actorToDetect = playerRef;
			}
			
			if(hitResult.GetActor() == actorToDetect)
			{
				memory->ownerEnemy->GetAIController()->StopMovement();
				
				hasDetected = true;
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
