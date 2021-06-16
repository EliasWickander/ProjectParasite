#include "DevUtils.h"

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