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

	if(amountEnemiesLeft <= 0)
	{
		OnGameWon();
	}
}

void AEliminationGamemode::OnPlayerDeath(APawnBase* deadPlayer)
{
	OnGameLost();
}
