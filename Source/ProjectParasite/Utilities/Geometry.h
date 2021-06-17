#pragma once

struct SCone
{
	FVector originPoint;
	FQuat rotation;
	float angle;
	float height;
	float range;
};

struct SConeVertices
{
	FVector originTopPoint;
	FVector originBotPoint;
	FVector topLeftFrontEnd;
	FVector topRightFrontEnd;
	FVector botLeftFrontEnd;
	FVector botRightFrontEnd;
};

struct SCube
{
	FVector center;
	FVector extents;
	FQuat rotation;
};

SConeVertices GetConeVerticesFromData(SCone cone);
SCube GetCubeContainingCone(SCone cone);