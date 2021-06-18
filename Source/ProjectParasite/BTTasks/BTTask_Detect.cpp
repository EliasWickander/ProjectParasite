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

	BTTaskDetectMemory* memory = reinterpret_cast<BTTaskDetectMemory*>(NodeMemory);
	
	memory->ownerEnemy = OwnerComp.GetAIOwner()->GetPawn<APawnEnemy>();

	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	memory->blackboard = OwnerComp.GetBlackboardComponent();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Detect::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	Detect(NodeMemory);
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
			if(hitResult.GetActor() == playerRef)
			{
				memory->blackboard->SetValueAsEnum("CurrentState", (uint8)ShooterStates::State_Chase);	
			}
			else
			{
				//Vision obstructed by something in player's path
				//blackboard->ClearValue("PlayerLocation");
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Didn't hit"));
		}
	}
	else
	{
		//blackboard->ClearValue("PlayerLocation");
	}
}

uint16 UBTTask_Detect::GetInstanceMemorySize() const
{
	return sizeof(BTTaskDetectMemory);
}
