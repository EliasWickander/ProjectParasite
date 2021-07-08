// Fill out your copyright notice in the Description page of Project Settings.


#include "EliminationGamemode.h"

#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Actors/Weapons/WeaponBase.h"
#include "ProjectParasite/DamageTypes/DamageType_Environmental.h"
#include "ProjectParasite/Actors/GoalTrigger.h"
#include "ProjectParasite/GameManager.h"

AEliminationGamemode::AEliminationGamemode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEliminationGamemode::BeginPlay()
{
	Super::BeginPlay();

	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	gameManagerRef = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	playerRef->onStartDeathEvent.AddDynamic(this, &AEliminationGamemode::OnPlayerDeath);

	gameManagerRef->OnFloorEnterEvent.AddDynamic(this, &AEliminationGamemode::OnFloorEnter);
	gameManagerRef->OnFloorExitEvent.AddDynamic(this, &AEliminationGamemode::OnFloorExit);

	OnFloorEnter();
}

void AEliminationGamemode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(levelTimer < levelTimeLimit)
	{
		levelTimer += DeltaSeconds;
	}

	if(comboTimer > 0)
	{
		comboTimer -= DeltaSeconds;
	}
}

void AEliminationGamemode::OnFloorEnter()
{	
	//When an enemy dies, call the OnEnemyDeath method
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnEnemy::StaticClass(), enemiesAlive);

	for(AActor* enemy : enemiesAlive)
	{
		Cast<APawnEnemy>(enemy)->onStartDeathEvent.AddDynamic(this, &AEliminationGamemode::OnEnemyDeath);
	}
	
	TArray<AActor*> goalTriggers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoalTrigger::StaticClass(), goalTriggers);

	UE_LOG(LogTemp, Warning, TEXT("%i"), goalTriggers.Num());
	
	if(goalTriggers.Num() > 0)
	{
		goalTrigger = Cast<AGoalTrigger>(goalTriggers[0]);

		goalTrigger->onGoalTriggered.AddDynamic(this, &AEliminationGamemode::OnGoalTriggered);	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No goal triggers on floor"));
	}
}

void AEliminationGamemode::OnFloorExit()
{
	for(AActor* enemy : enemiesAlive)
	{
		Cast<APawnEnemy>(enemy)->onStartDeathEvent.RemoveDynamic(this, &AEliminationGamemode::OnEnemyDeath);
	}

	if(goalTrigger)
	{
		goalTrigger->onGoalTriggered.RemoveDynamic(this, &AEliminationGamemode::OnGoalTriggered);		
	}
}

bool AEliminationGamemode::HasEliminatedAllEnemies()
{
	//If there are no enemies left (excluding a possessed one), the game is won
	
	int targetAmountEnemiesLeft = 0;

	if(enemiesAlive.Num() == 1 && playerRef->GetPossessedEnemy())
	{
		if(playerRef->GetPossessedEnemy() == enemiesAlive[0])
		{
			targetAmountEnemiesLeft = 1;
		}	
	}

	return enemiesAlive.Num() <= targetAmountEnemiesLeft;
}

void AEliminationGamemode::OnEnemyDeath(APawnBase* deadEnemy, AActor* causerActor)
{
	Cast<APawnEnemy>(deadEnemy)->onStartDeathEvent.RemoveDynamic(this, &AEliminationGamemode::OnEnemyDeath);
	
	enemiesAlive.Remove(deadEnemy);

	if(causerActor != nullptr)
	{
		if(Cast<APawnEnemy>(causerActor))
		{
			if(playerRef->GetPossessedEnemy() == causerActor)
			{
				AWeaponBase* weapon = playerRef->GetPossessedEnemy()->GetWeapon();

				AddScore(weapon->GetKillScore());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Causer actor is not possessed enemy"));
			}
		}
		else if(Cast<APawnParasite>(causerActor))
		{
			AddScore(playerRef->GetKillScore());
		}
		
		if(HasEliminatedAllEnemies())
		{
			OnGameWon();
		}	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Causer actor is null"));
	}
}

void AEliminationGamemode::OnPlayerDeath(APawnBase* deadPlayer, AActor* causerActor)
{
	//When player dies, game is lost
	OnGameLost();
}

void AEliminationGamemode::OnGoalTriggered()
{
	//if(HasEliminatedAllEnemies())
	//{
		if(!gameManagerRef->IsCurrentFloorLast())
		{
			gameManagerRef->LoadNextFloor();		
		}
		else
		{
			//Add score if player finished level before time limit
			if(levelTimer < levelTimeLimit)
				AddScore(finishOnTimeScore, false);

			UGameplayStatics::OpenLevel(GetWorld(), "Hideout");
		}
	//}
}

void AEliminationGamemode::AddScore(float scoreToAdd, bool allowCombo)
{
	currentScore += scoreToAdd;

	UE_LOG(LogTemp, Warning, TEXT("Added %f score"), scoreToAdd);

	if(allowCombo)
	{
		if(comboTimer > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Combo! Added %f score"), comboScore);
			currentScore += comboScore;	
		}

		comboTimer = comboWindow;
	}
}
