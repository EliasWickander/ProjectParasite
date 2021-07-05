// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceCustom.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/GameStateCustom.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"


void UGameInstanceCustom::TransferEnemyToNextLevel(APawnEnemy* enemy)
{
	// EnemySpawnData enemyToTransfer;
	//
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *enemy->GetClass()->GetPathName());
	//
	// enemyToTransfer.enemyType = enemy->StaticClass();
	// enemyToTransfer.location = enemy->GetActorLocation();
	// enemyToTransfer.rotation = enemy->GetActorRotation();
	//
	// FString bpPath = enemy->StaticClass()->GetName();
	//
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *bpPath);
	//
	// enemiesToTransfer.Add(enemyToTransfer);
}

void UGameInstanceCustom::OnFloorEnter()
{
	// for(EnemySpawnData enemyToTransfer : enemiesToTransfer)
	// {
	// 	GetWorld()->SpawnActor<APawnEnemy>(enemyToTransfer.enemyType, enemyToTransfer.location, enemyToTransfer.rotation);
	// }
}
