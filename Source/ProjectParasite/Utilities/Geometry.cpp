#include "Geometry.h"
#include "ProjectParasite/Utilities/DevUtils.h"

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