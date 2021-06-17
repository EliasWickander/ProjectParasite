#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "ProjectParasite/Utilities/Geometry.h"

#define ENABLE_DRAW_DEBUG  !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

#if ENABLE_DRAW_DEBUG

void DrawCone(SCone cone, const UWorld* inWorld, FColor color);

SConeVertices GetConeVerticesFromData(SCone cone);
SCube GetCubeContainingCone(SCone cone);

#endif