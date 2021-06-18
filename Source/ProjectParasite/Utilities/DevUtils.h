#pragma once

#include "ProjectParasite/Utilities/Geometry.h"

class UBTTaskNode;
class UBehaviorTree;
class UBTCompositeNode;

FVector AngleVector(float deg);
	
float AngleBetweenVectors(FVector from, FVector to);

bool OverlapCone(SCone cone, const UObject* worldContextObject, const TArray<TEnumAsByte<EObjectTypeQuery>>& objectTypes, UClass* actorClassFilter, const TArray<AActor*>& actorsToIgnore, TArray<AActor*>& outActors);

//Find all BTTasks in behavior tree
TArray<UBTTaskNode*> FindAllTasksInBehaviorTree(UBehaviorTree* tree);

//Find all BTTasks that are children of this node
TArray<UBTTaskNode*> FindAllTaskChildrenOfNode(UBTCompositeNode* node);

//Find task of a specific type in behavior tree

template <class TaskType>
TaskType* FindTaskInTree(UBehaviorTree* tree)
{
	UClass* taskType = TaskType::StaticClass();

	for(UBTTaskNode* node : FindAllTasksInBehaviorTree(tree))
	{
		TaskType* nodeCasted = Cast<TaskType>(node);

		if(nodeCasted)
		{
			if(nodeCasted->StaticClass() == taskType)
			{
				return nodeCasted;
			}
		}
	}

	return nullptr;
}