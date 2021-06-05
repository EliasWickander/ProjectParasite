// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnParasite.h"

#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "EngineUtils.h"
#include "ProjectParasite/Components/Debug/ParasiteDebugComponent.h"
#include "ProjectParasite/PlayerControllers/ParasitePlayerController.h"
#include "ProjectParasite/Utilities/StateMachine/ParasiteStateMachine.h"
#include "ProjectParasite/Utilities/StateMachine/States/Player/Player_State_Possess.h"

APawnParasite::APawnParasite()
{
	parasiteDebugger = CreateDefaultSubobject<UParasiteDebugComponent>(TEXT("Debug Component"));
	stateMachine = CreateDefaultSubobject<UParasiteStateMachine>(TEXT("State Machine"));
}

// Called when the game starts or when spawned
void APawnParasite::BeginPlay()
{
	Super::BeginPlay();
	
	if(playerControllerRef)
	{
		playerControllerRef->SetPlayerInputEnabled(true);
	}
	
}

// Called every frame
void APawnParasite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APawnParasite::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Possess", EInputEvent::IE_Pressed, this, &APawnParasite::PossessClosestEnemyInRadius);
	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Pressed, this, &APawnParasite::Dash);
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

	stateMachine->possessState->Init(enemiesInRadius[0]);
	stateMachine->SetState("State_Possess");
}