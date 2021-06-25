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
	TArray<AActor*> allEnemiesInWorld;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnEnemy::StaticClass(), allEnemiesInWorld);

	amountEnemiesLeft = allEnemiesInWorld.Num();
	
	for(AActor* enemy : allEnemiesInWorld)
	{
		Cast<APawnEnemy>(enemy)->onStartDeathEvent.AddDynamic(this, &AEliminationGamemode::OnEnemyDeath);
	}
}

void AEliminationGamemode::OnEnemyDeath(APawnBase* deadEnemy)
{
	Cast<APawnEnemy>(deadEnemy)->onStartDeathEvent.RemoveDynamic(this, &AEliminationGamemode::OnEnemyDeath);
	
	amountEnemiesLeft--;

	//If there are no enemies left (excluding a possessed one), the game is won
	int targetAmountEnemiesLeft = playerRef->GetPossessedEnemy() ? 1 : 0;
	
	if(amountEnemiesLeft <= targetAmountEnemiesLeft)
	{
		OnGameWon();
	}	
}

void AEliminationGamemode::OnPlayerDeath(APawnBase* deadPlayer)
{
	//When player dies, game is lost
	OnGameLost();
}
