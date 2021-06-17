#pragma once

#include "ProjectParasite/Utilities/Geometry.h"

FVector AngleVector(float deg);
	
float AngleBetweenVectors(FVector from, FVector to);

bool OverlapCone(SCone cone, const UObject* worldContextObject, const TArray<TEnumAsByte<EObjectTypeQuery>>& objectTypes, UClass* actorClassFilter, const TArray<AActor*>& actorsToIgnore, TArray<AActor*>& outActors);