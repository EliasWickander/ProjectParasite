// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnShooter.h"
#include "Components/CapsuleComponent.h"

#include "ProjectParasite/Utilities/StateMachine/StateMachine.h"

#include "ProjectParasite/Utilities/StateMachine/States/SState_Idle.h"

APawnShooter::APawnShooter()
{
	
	
}

void APawnShooter::BeginPlay()
{
	Super::BeginPlay();

	stateMachine = new StateMachine("State_Idle", new SState_Idle(this));
	int t = 1;
}

void APawnShooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	stateMachine->Update();	
}
