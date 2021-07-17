// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateCustom.h"

#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/GameManager.h"
#include "ProjectParasite/Pawns/PawnParasite.h"

AGameStateCustom::AGameStateCustom()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void AGameStateCustom::BeginPlay()
{
	Super::BeginPlay();

	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	gameManagerRef = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	gameManagerRef->BeginPlay();
}

void AGameStateCustom::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	gameManagerRef->Tick(DeltaSeconds);
}