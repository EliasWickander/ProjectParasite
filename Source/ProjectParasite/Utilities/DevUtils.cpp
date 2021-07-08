#include "DevUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Engine/LevelStreaming.h"

FVector AngleVector(float deg)
{
	float angleInRad = FMath::DegreesToRadians(deg);

	return FVector(FMath::Cos(angleInRad), FMath::Sin(angleInRad), 0);
}

float AngleBetweenVectors(FVector from, FVector to)
{
	float dot = FVector::DotProduct(from, to);
	float angleInDeg = FMath::RadiansToDegrees(acos(dot));

	return angleInDeg;
}

bool OverlapCone(SCone cone, const UObject* worldContextObject,
	const TArray<TEnumAsByte<EObjectTypeQuery>>& objectTypes, UClass* actorClassFilter,
	const TArray<AActor*>& actorsToIgnore, TArray<AActor*>& outActors)
{
	outActors.Empty();

	SCube cubeData = GetCubeContainingCone(cone);

	//Check if box is overlapping player
	TArray<AActor*> overlappedActors;

	UKismetSystemLibrary::BoxOverlapActors(
		worldContextObject,
		cubeData.center,
		cubeData.extents,
		objectTypes,
		actorClassFilter,
		actorsToIgnore,
		overlappedActors
		);

	//Find the angle to each overlapped actors position
	for(AActor* overlappedActor : overlappedActors)
	{
		FVector dirToActor = overlappedActor->GetActorLocation() - cone.originPoint;
		dirToActor.Z = 0;
		dirToActor.Normalize();

		float angle = AngleBetweenVectors(cone.rotation * FVector::ForwardVector, dirToActor);

		//If players angle matches the cone's angle, it is considered inside of the cone
		if(angle <= cone.angle * 0.5f)
		{
			outActors.Push(overlappedActor);
		}
	}

	//Returns true if cone overlaps at least one actor
	return outActors.Num() > 0;
}

TArray<UBTTaskNode*> FindAllTasksInBehaviorTree(UBehaviorTree* tree)
{
	TArray<UBTTaskNode*> taskNodes = FindAllTaskChildrenOfNode(tree->RootNode);

	return taskNodes;
}

TArray<UBTTaskNode*> FindAllTaskChildrenOfNode(UBTCompositeNode* node)
{
	TArray<UBTTaskNode*> taskNodes;
	
	for(FBTCompositeChild child : node->Children)
	{
		if(child.ChildTask)
		{
			taskNodes.Push(child.ChildTask);
		}
		else
		{
			taskNodes.Append(FindAllTaskChildrenOfNode(child.ChildComposite));
		}
	}

	return taskNodes;
}

bool SetFocusExtended(AAIController* AIController, AActor* targetActor, float rotSpeed, float acceptanceDist)
{
	APawn* AIPawn = AIController->GetPawn();
	
	FVector dirToTarget = targetActor->GetActorLocation() - AIPawn->GetActorLocation();

	dirToTarget.Normalize();

	//Interpolate between owners forward dir and target dir
	FVector finalFocalPoint = FMath::Lerp(AIPawn->GetActorForwardVector(), dirToTarget, rotSpeed * AIPawn->GetWorld()->GetDeltaSeconds());

	//Set ai to look in final direction
	AIController->SetFocalPoint(AIPawn->GetActorLocation() + finalFocalPoint);

	//Returns true if distance between final focal point and target dir is less than acceptance dist
	return FVector::Dist(finalFocalPoint, dirToTarget) <= acceptanceDist;
}

bool SetFocalPointExtended(AAIController* AIController, FVector targetPoint, float rotSpeed, float acceptanceDist)
{
	APawn* AIPawn = AIController->GetPawn();
	
	FVector dirToTarget = targetPoint - AIPawn->GetActorLocation();

	dirToTarget.Normalize();

	//Interpolate between owners forward dir and target dir
	FVector finalFocalPoint = FMath::Lerp(AIPawn->GetActorForwardVector(), dirToTarget, rotSpeed * AIPawn->GetWorld()->GetDeltaSeconds());

	//Set ai to look in final direction
	AIController->SetFocalPoint(AIPawn->GetActorLocation() + finalFocalPoint);

	//Returns true if distance between final focal point and target dir is less than acceptance dist
	return FVector::Dist(finalFocalPoint, dirToTarget) <= acceptanceDist;
}

void MoveActorToLevel(AActor* actorToMove, ULevelStreaming* fromLevel, ULevelStreaming* toLevel)
{
	ULevel* fromLoadedLevel = fromLevel->GetLoadedLevel();
	ULevel* toLoadedLevel = toLevel->GetLoadedLevel();
	
	if(fromLoadedLevel && toLoadedLevel)
	{
		if(fromLoadedLevel->Actors.Find(actorToMove))
		{
			actorToMove->Rename(nullptr, toLoadedLevel);
			toLoadedLevel->Actors.Add(actorToMove);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Level %s does not contain actor %s"), *fromLoadedLevel->GetName(), *actorToMove->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("From or to level is not loaded. Cannot access"));
	}
}
