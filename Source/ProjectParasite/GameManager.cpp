// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include "Engine/LevelStreaming.h"
#include "GameFramework/PlayerStart.h"
#include "ProjectParasite/GameModes/EliminationGamemode.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Pawns/PawnEnemy.h"
#include "Utilities/DevUtils.h"
#include "Utilities/StateMachine/StateMachine.h"
#include "Utilities/StateMachine/States/Player/Player_State_Possess.h"

UGameManager::UGameManager()
{
	PrepareLevelMap();
}

void UGameManager::BeginPlay()
{
	levelsDirectoryPath = FString::Printf(TEXT("%s/Levels"), *FPaths::ProjectContentDir());

	gamemodeRef = Cast<AEliminationGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	currentWorldName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if(!beginPlayTriggered)
	{
		if(IsOnFloorLevel())
			gamemodeRef->OnFloorEnter(GetCurrentFloor());
		
		beginPlayTriggered = true;	
	}
}

void UGameManager::Tick(float DeltaSeconds)
{
	if(nextLevel != currentLevel)
	{
		if(*GetWorld()->GetName() == nextLevel)
		{
			currentLevel = nextLevel;
			
			if(possessedEnemyToTransition != nullptr)
			{
				AActor* spawnedClone = GetWorld()->SpawnActor<AActor>(possessedEnemyToTransition, playerRef->GetActorLocation(), playerRef->GetActorRotation());

				APawnEnemy* spawnedCloneAsEnemy = Cast<APawnEnemy>(spawnedClone);
				playerRef->possessState->SetPossessedEnemy(spawnedCloneAsEnemy, true);
				playerRef->stateMachine->SetState("State_Possess");
			}
			
			if(IsOnFloorLevel())
			{
				OnFloorEnterEvent.Broadcast(GetCurrentFloor());
				OnFloorEnter();	
			}
		}
	}
}

void UGameManager::OpenLevel(int level, int floor)
{
	
	//Opens level if it exists
	if(DoesLevelExist(level, floor))
	{
		if(playerRef->GetPossessedEnemy())
			possessedEnemyToTransition = playerRef->GetPossessedEnemy()->GetClass();
		
		FString levelName = FString::Printf(TEXT("Level_%i_%i"), level, floor);

		if(IsOnFloorLevel())
		{
			OnFloorExitEvent.Broadcast(GetCurrentFloor());
			OnFloorExit();
		}
		
		UGameplayStatics::OpenLevel(GetWorld(), *levelName);

		nextLevel = levelName;;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to open level %i floor %i that doesn't exist"), level, floor);
	}
}

void UGameManager::SpawnCopyOfPossessedEnemy(FVector pos)
{
	if(playerRef->GetPossessedEnemy())
	{
		FVector position = pos;
		FRotator rotation = FRotator::ZeroRotator;
		
		//find a way to spawn copy of blueprint of possessed enemy
		AActor* spawnedClone = GetWorld()->SpawnActor<AActor>(playerRef->GetPossessedEnemy()->GetClass(), position, rotation);
	}
}


bool UGameManager::DoesLevelExist(int level, int floor)
{
	FString levelToCheck = GetSubLevelName(level, floor);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *levelToCheck);
	return FPaths::FileExists(FString::Printf(TEXT("%s/%s.umap"), *levelsDirectoryPath, *levelToCheck));
}

int UGameManager::GetLevelAmount()
{
	return levelMap.Num();
}

int UGameManager::GetFloorAmount(int level)
{
	if(levelMap.Contains(level))
	{
		return levelMap[level].Num();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to get floor amount of level %i that doesn't exist"), level);
		return -1;
	}
}

bool UGameManager::IsCurrentFloorLast()
{
	return !DoesLevelExist(GetCurrentLevel(), GetCurrentFloor() + 1);
}

void UGameManager::LoadNextFloor()
{
	UE_LOG(LogTemp, Warning, TEXT("load next floor"));
	OpenLevel(GetCurrentLevel(), GetCurrentFloor() + 1);
}

bool UGameManager::IsOnFloorLevel()
{
	return GetCurrentLevel() != 0;
}

void UGameManager::RestartFloor()
{
}

void UGameManager::PrepareLevelMap()
{
	//Add all available levels and floors to a map of arrays
	//The key represents the level, and the array its floors
	
	int levelAmount = 0;

	while(FPaths::FileExists(FString::Printf(TEXT("%s/Level_%i.umap"), *levelsDirectoryPath, levelAmount + 1)))
	{
		int floorAmount = 0;
		TArray<int> floorArray;

		while(FPaths::FileExists(FString::Printf(TEXT("%s/Level_%i_%i.umap"), *levelsDirectoryPath, levelAmount + 1, floorAmount + 1)))
		{
			floorArray.Add(floorAmount);

			floorAmount++;
		}
		
		levelAmount++;
	}
}

FString UGameManager::GetSubLevelName(int level, int floor)
{
	return FString::Printf(TEXT("Level_%i_%i"), level, floor);
}

int UGameManager::GetCurrentFloor()
{
	FString levelName = *GetWorld()->GetName();
	FString prefix;
	FString content;
	
	levelName.Split(TEXT("_"), &prefix, &content);

	FString level;
	FString floor;

	levelName.Split(TEXT("_"), &level, &floor);

	return UKismetStringLibrary::Conv_StringToInt(floor);
}

int UGameManager::GetCurrentLevel()
{
	FString levelName = *GetWorld()->GetName();
	FString prefix;
	FString content;
	
	levelName.Split(TEXT("_"), &prefix, &content);

	FString level;
	FString floor;

	content.Split(TEXT("_"), &level, &floor);

	return UKismetStringLibrary::Conv_StringToInt(level);
}
