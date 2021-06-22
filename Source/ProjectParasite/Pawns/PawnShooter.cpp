// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnShooter.h"

APawnShooter::APawnShooter()
{
}

void APawnShooter::BeginPlay()
{
	Super::BeginPlay();
}

void APawnShooter::UpdatePawnBehavior(float deltaSeconds)
{
	if(isPendingDeath)
		return;
	
	Super::UpdatePawnBehavior(deltaSeconds);
}
