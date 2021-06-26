// Fill out your copyright notice in the Description page of Project Settings.


#include "EliminationGamemode.h"

#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "Kismet/GameplayStatics.h"

void AEliminationGamemode::BeginPlay()
{
	Super::BeginPlay();

	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	playerRef->onStartDeathEvent.AddDynamic(this, &AEliminationGamemode::OnPlayerDeath);

	//When an enemy dies, call the OnEnemyDeath method
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnEnemy::StaticClass(), enemiesAlive);

	
	for(AActor* enemy : enemiesAlive)
	{
		Cast<APawnEnemy>(enemy)->onStartDeathEvent.AddDynamic(this, &AEliminationGamemode::OnEnemyDeath);
	}
}

void AEliminationGamemode::OnEnemyDeath(APawnBase* deadEnemy)
{
	Cast<APawnEnemy>(deadEnemy)->onStartDeathEvent.RemoveDynamic(this, &AEliminationGamemode::OnEnemyDeath);

	enemiesAlive.Remove(deadEnemy);

	//If there are no enemies left (excluding a possessed one), the game is won

	int targetAmountEnemiesLeft = 0;

	if(enemiesAlive.Num() == 1 && playerRef->GetPossessedEnemy())
	{
		if(playerRef->GetPossessedEnemy() == enemiesAlive[0])
		{
			targetAmountEnemiesLeft = 1;
		}	
	}

	if(enemiesAlive.Num() <= targetAmountEnemiesLeft)
	{
		OnGameWon();
	}	
}

void AEliminationGamemode::OnPlayerDeath(APawnBase* deadPlayer)
{
	//When player dies, game is lost
	OnGameLost();
}
