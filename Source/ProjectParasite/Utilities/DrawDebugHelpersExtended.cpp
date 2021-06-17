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