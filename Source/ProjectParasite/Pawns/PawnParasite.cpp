// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnParasite.h"

#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "EngineUtils.h"
#include "ProjectParasite/Components/Debug/ParasiteDebugComponent.h"
#include "ProjectParasite/Utilities/StateMachine/StateMachine.h"
#include "ProjectParasite/Utilities/StateMachine/States/Player/Player_State_Idle.h"
#include "ProjectParasite/Utilities/StateMachine/States/Player/Player_State_Dash.h"
#include "ProjectParasite/Utilities/StateMachine/States/Player/Player_State_Possess.h"

APawnParasite::APawnParasite()
{
	parasiteDebugger = CreateDefaultSubobject<UParasiteDebugComponent>(TEXT("Debug Component"));
}

void APawnParasite::BeginPlay()
{
	Super::BeginPlay();

	stateMachine = NewObject<UStateMachine>();
	stateMachine->SetOwner(this);
	
	idleState = NewObject<UPlayer_State_Idle>();
	dashState = NewObject<UPlayer_State_Dash>();
	possessState = NewObject<UPlayer_State_Possess>();
	
	stateMachine->AddState(TEXT("State_Idle"), idleState);
	stateMachine->AddState(TEXT("State_Dash"), dashState);
	stateMachine->AddState(TEXT("State_Possess"), possessState);
}

void APawnParasite::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

	stateMachine->Update();
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetController()->GetName())
}

void APawnParasite::Dash()
{
	stateMachine->SetState("State_Dash");
}

void APawnParasite::PossessClosestEnemyInRadius()
{
	TArray<APawnEnemy*> enemiesInRadius;

	//Find all enemies in world that are in radius of player
 	for (TActorIterator<APawnEnemy> enemy(GetWorld()); enemy; ++enemy)
	{
		APawnEnemy* enemyFound = *enemy;

 		if(FVector::Dist(enemy->GetActorLocation(), GetActorLocation()) <= possessRadius)
 			enemiesInRadius.Add(enemyFound);
	}

	//If there are enemies in radius, possess the first one
	if(enemiesInRadius.Num() <= 0)
		return;

	//Save reference to enemy player is possessing
	possessedEnemy = enemiesInRadius[0];
	stateMachine->SetState("State_Possess");
}

APawnEnemy* APawnParasite::GetPossessedEnemy()
{
	return possessedEnemy;
}

float APawnParasite::GetPossessRadius()
{
	return possessRadius;
}
