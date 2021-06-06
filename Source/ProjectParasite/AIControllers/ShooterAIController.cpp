// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "ProjectParasite/Pawns/PawnParasite.h"
#include "Kismet/GameplayStatics.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	playerRef = Cast<APawnParasite>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}
