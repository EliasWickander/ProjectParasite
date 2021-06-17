#include "DevUtils.h"
#include "Kismet/KismetSystemLibrary.h"

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