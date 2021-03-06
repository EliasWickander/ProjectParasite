// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnShooter.h"
#include "ProjectParasite/Components/Debug/EnemyDebugComponent.h"

APawnShooter::APawnShooter()
{
	enemyDebugger = CreateDefaultSubobject<UEnemyDebugComponent>(TEXT("Debug Component"));
}

void APawnShooter::BeginPlay()
{
	Super::BeginPlay();
}

void APawnShooter::UpdatePawnBehavior(float deltaSeconds)
{
	//Don't update pawn if it's dying
	if(isPendingDeath)
		return;
	
	Super::UpdatePawnBehavior(deltaSeconds);
}
