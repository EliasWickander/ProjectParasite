// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include "AIController.h"
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
#include "BehaviorTree/BehaviorTree.h"
#include "Camera/CameraComponent.h"
#include "Components/PostProcessComponent.h"
#include "ProjectParasite/ScoreHandler.h"

//Called on game state begin play (when a new level loads)
void UGameManager::BeginPlay()
{
	gamemodeRef = Cast<AEliminationGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	currentWorldName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	//if level loaded is a floor level, call enter event
	if(IsOnFloorLevel())
	{
		transitionIntoLevel = true;
		OnFloorEnterEvent.Broadcast(GetCurrentFloor());
		OnFloorEnter();	
	}

	isPaused = false;
	beginPlayTriggered = true;

	if(totalScoreHandler == nullptr)
	{
		totalScoreHandler = NewObject<UScoreHandler>();	
	}
	
	if(GetCurrentFloor() == 1)
		OnLevelStart();
	
	OnBeginPlay();
}

void UGameManager::Tick(float DeltaSeconds)
{
	if(beginPlayTriggered)
	{
		//Handle enemy transition after the first frame to prevent conflicts
		if(IsOnFloorLevel())
		{	
			if(possessedEnemyToTransition != nullptr)
			{
				AActor* spawnedClone = GetWorld()->SpawnActor<AActor>(possessedEnemyToTransition, playerRef->GetActorLocation(), playerRef->GetActorRotation());
				
				APawnEnemy* spawnedCloneAsEnemy = Cast<APawnEnemy>(spawnedClone);

				if(spawnedCloneAsEnemy)
				{
					//Possess the new instance of the enemy
					playerRef->possessState->SetPossessedEnemy(spawnedCloneAsEnemy, true);
					playerRef->stateMachine->SetState("State_Possess");	
				}
			}

			SetPaused(true);
		}
		
		beginPlayTriggered = false;
	}

	if(transitionOutOfLevel)
	{
		if(FMath::Clamp<float>(levelTransitionTimer += DeltaSeconds, 0, levelTransitionTime) == levelTransitionTime)
		{
			levelTransitionTimer = 0;
			transitionOutOfLevel = false;
			UGameplayStatics::OpenLevel(GetWorld(), *nextLevel);	
		}
		
		//Transition to black
	}
	else if(transitionIntoLevel)
	{
		if(FMath::Clamp<float>(levelTransitionTimer += DeltaSeconds, 0, levelTransitionTime) == levelTransitionTime)
		{
			levelTransitionTimer = 0;
			transitionIntoLevel = false;
			SetPaused(false);
		}
		//Transition from black to game
	}

	if(levelTimer < gamemodeRef->GetLevelTimeLimit())
	{
		levelTimer += DeltaSeconds;
	}

	OnTick(DeltaSeconds);
}

void UGameManager::OpenLevel(int level, int floor)
{
	//Opens level if it exists
	if(DoesLevelExist(level, floor))
	{
		FString levelName = FString::Printf(TEXT("Level_%i_%i"), level, floor);
		
		//If a player is possessing an enemy when loading new level, save reference to that enemy
		if(playerRef->GetPossessedEnemy())
			possessedEnemyToTransition = playerRef->GetPossessedEnemy()->GetClass();

		//If player is exiting a floor level, call exit event
		if(IsOnFloorLevel())
		{
			OnFloorExitEvent.Broadcast(GetCurrentFloor());
			OnFloorExit();
		}

		if(IsOnFloorLevel())
		{
			SetPaused(true);
			nextLevel = levelName;
			transitionOutOfLevel = true;
		}
		else
		{
			UGameplayStatics::OpenLevel(GetWorld(), *levelName);	
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to open level %i floor %i that doesn't exist"), level, floor);
	}
}

void UGameManager::RestartLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), *currentWorldName);
}

bool UGameManager::DoesLevelExist(int level, int floor)
{
	FString levelsDirectoryPath = FString::Printf(TEXT("%s/Levels"), *FPaths::ProjectContentDir());
	FString levelToCheck = FString::Printf(TEXT("Level_%i_%i"), level, floor);;

	return FPaths::FileExists(FString::Printf(TEXT("%s/%s.umap"), *levelsDirectoryPath, *levelToCheck));
}

bool UGameManager::IsCurrentFloorLast()
{
	return !DoesLevelExist(GetCurrentLevel(), GetCurrentFloor() + 1);
}

void UGameManager::LoadNextFloor()
{
	OpenLevel(GetCurrentLevel(), GetCurrentFloor() + 1);
}

bool UGameManager::IsOnFloorLevel()
{
	return GetCurrentLevel() != 0;
}

int UGameManager::GetCurrentFloor()
{
	FString prefix;
	FString content;
	
	currentWorldName.Split(TEXT("_"), &prefix, &content);

	FString level;
	FString floor;

	content.Split(TEXT("_"), &level, &floor);

	return UKismetStringLibrary::Conv_StringToInt(floor);
}

int UGameManager::GetCurrentLevel()
{
	FString prefix;
	FString content;
	
	currentWorldName.Split(TEXT("_"), &prefix, &content);

	FString level;
	FString floor;

	content.Split(TEXT("_"), &level, &floor);

	return UKismetStringLibrary::Conv_StringToInt(level);
}

void UGameManager::OnLevelStart()
{
	levelTimer = 0;
	totalScoreHandler = NewObject<UScoreHandler>();	
}

void UGameManager::SetPaused(bool paused)
{
	if(isPaused == paused)
		return;
	
	if(paused)
	{
		TArray<AActor*> outActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIController::StaticClass(), outActors);

		for(int i = 0; i < outActors.Num(); i++)
		{
			AAIController* controller = Cast<AAIController>(outActors[i]);

			controller->GetBrainComponent()->StopLogic(FString("Pause"));
		}

		OnPauseGameEvent.Broadcast();
		OnPauseGame();
	}
	else
	{
		TArray<AActor*> outActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIController::StaticClass(), outActors);

		for(int i = 0; i < outActors.Num(); i++)
		{
			AAIController* controller = Cast<AAIController>(outActors[i]);

			if(controller->GetPawn() != nullptr)
				controller->GetBrainComponent()->StartLogic();
		}

		OnUnpauseGameEvent.Broadcast();
		OnUnpauseGame();
	}

	isPaused = paused;
}
