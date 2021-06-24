// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameMode.h"

#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "Kismet/GameplayStatics.h"

void ACustomGameMode::BeginPlay()
{
	Super::BeginPlay();

	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	playerRef->onStartDeathEvent.AddDynamic(this, &ACustomGameMode::OnPlayerDeath);
	
	TArray<AActor*> allEnemiesInWorld;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnEnemy::StaticClass(), allEnemiesInWorld);

	amountEnemiesLeft = allEnemiesInWorld.Num();
	
	for(AActor* enemy : allEnemiesInWorld)
	{
		Cast<APawnEnemy>(enemy)->onStartDeathEvent.AddDynamic(this, &ACustomGameMode::OnEnemyDeath);
	}
}

void ACustomGameMode::OnEnemyDeath(APawnBase* deadEnemy)
{
	Cast<APawnEnemy>(deadEnemy)->onStartDeathEvent.RemoveDynamic(this, &ACustomGameMode::OnEnemyDeath);
	
	amountEnemiesLeft--;

	if(amountEnemiesLeft <= 0)
	{
		OnGameWon();
	}
}

void ACustomGameMode::OnPlayerDeath(APawnBase* deadPlayer)
{
	OnGameLost();
}
