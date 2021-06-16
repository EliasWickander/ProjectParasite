#include "DrawDebugHelpersExtended.h"
#include "DrawDebugHelpers.h"
#include "ProjectParasite/Utilities/DevUtils.h"

void DrawCone(SCone cone, const UWorld* inWorld, FColor color)
{
	SConeVertices vertices = GetConeVerticesFromData(cone);

	//Draw the four lines making up the cone's shape
	DrawDebugLine(inWorld, vertices.originTopPoint, vertices.topRightFrontEnd, color);
	DrawDebugLine(inWorld, vertices.originTopPoint, vertices.topLeftFrontEnd, color);
	DrawDebugLine(inWorld, vertices.originBotPoint, vertices.botRightFrontEnd, color);
	DrawDebugLine(inWorld, vertices.originBotPoint, vertices.botLeftFrontEnd, color);

	//Draw the four lines connecting the four above
	DrawDebugLine(inWorld, vertices.botRightFrontEnd, vertices.topRightFrontEnd, color);
	DrawDebugLine(inWorld, vertices.botLeftFrontEnd, vertices.topLeftFrontEnd, color);
	DrawDebugLine(inWorld, vertices.topLeftFrontEnd, vertices.topRightFrontEnd, color);
	DrawDebugLine(inWorld, vertices.botLeftFrontEnd, vertices.botRightFrontEnd, color);
}

SConeVertices GetConeVerticesFromData(SCone cone)
{
	SConeVertices verticesStruct;

	float halfHeight = cone.height * 0.5f;
	float halfAngle = cone.angle * 0.5f;

	FTransform rotatedTransform(cone.rotation);
	
	verticesStruct.originTopPoint = cone.originPoint + FVector(0, 0, halfHeight);
	verticesStruct.originBotPoint = cone.originPoint - FVector(0, 0, halfHeight);
	
	verticesStruct.topRightFrontEnd = verticesStruct.originTopPoint + rotatedTransform.TransformPosition(AngleVector(halfAngle)) * cone.range;
	verticesStruct.topLeftFrontEnd = verticesStruct.originTopPoint + rotatedTransform.TransformPosition(AngleVector(-halfAngle)) * cone.range;
	verticesStruct.botRightFrontEnd = verticesStruct.originBotPoint + rotatedTransform.TransformPosition(AngleVector(halfAngle)) * cone.range;
	verticesStruct.botLeftFrontEnd = verticesStruct.originBotPoint + rotatedTransform.TransformPosition(AngleVector(-halfAngle)) * cone.range;

	return verticesStruct;
	
}

SCube GetCubeContainingCone(SCone cone)
{
	SConeVertices coneVertices = GetConeVerticesFromData(cone);
	
	FVector topFrontCenter = (coneVertices.topRightFrontEnd + coneVertices.topLeftFrontEnd) / 2;
	
	FVector centerPoint = (topFrontCenter + coneVertices.originTopPoint) / 2;

	float centeredHeight = ((coneVertices.topRightFrontEnd + coneVertices.botRightFrontEnd) / 2).Z;
	centerPoint.Z = centeredHeight;
	
	float coneDepth = (topFrontCenter - coneVertices.originTopPoint).Size();
	float coneWidth = (coneVertices.topLeftFrontEnd - coneVertices.topRightFrontEnd).Size();
	float coneHeight = (coneVertices.topLeftFrontEnd - coneVertices.botLeftFrontEnd).Size();
	
	FVector extents(coneDepth / 2, coneWidth / 2,  coneHeight / 2);

	return SCube {centerPoint, extents, cone.rotation};
}