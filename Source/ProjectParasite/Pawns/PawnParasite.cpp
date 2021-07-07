// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnParasite.h"

#include "ProjectParasite/AIControllers/AIControllerBase.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Components/Debug/ParasiteDebugComponent.h"
#include "ProjectParasite/DamageTypes/DamageType_Environmental.h"
#include "ProjectParasite/Utilities/StateMachine/StateMachine.h"
#include "ProjectParasite/Utilities/StateMachine/States/Player/Player_State_Idle.h"
#include "ProjectParasite/Utilities/StateMachine/States/Player/Player_State_Dash.h"
#include "ProjectParasite/Utilities/StateMachine/States/Player/Player_State_Possess.h"

#include "ProjectParasite/PlayerControllers/PlayerControllerBase.h"

APawnParasite::APawnParasite()
{
	parasiteDebugger = CreateDefaultSubobject<UParasiteDebugComponent>(TEXT("Debug Component"));

	stateMachine = CreateDefaultSubobject<UStateMachine>(TEXT("State Machine"));
	stateMachine->SetupAttachment(RootComponent);
	stateMachine->SetOwner(this);

	idleState = CreateDefaultSubobject<UPlayer_State_Idle>(TEXT("State_Idle"));
	dashState = CreateDefaultSubobject<UPlayer_State_Dash>(TEXT("State_Dash"));
	possessState = CreateDefaultSubobject<UPlayer_State_Possess>(TEXT("State_Possess"));

	stateMachine->AddState(TEXT("State_Idle"), idleState);
	stateMachine->AddState(TEXT("State_Dash"), dashState);
	stateMachine->AddState(TEXT("State_Possess"), possessState);
}

void APawnParasite::BeginPlay()
{
	Super::BeginPlay();

	SetMoveSpeed(baseMoveSpeed);
}

void APawnParasite::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

	dashTimer = FMath::Clamp<float>(dashTimer - DeltaTime, 0, dashCooldown);
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

	possessState->possessedEnemy = enemiesInRadius[0];
	stateMachine->SetState("State_Possess");
}

void APawnParasite::SetPossessed(APawnEnemy* actorToPossess)
{
	//If player is already possessing enemy
	if(GetPossessedEnemy() != nullptr)
	{
		GetPossessedEnemy()->isPossessed = false;
		//Give them AI behavior again
		GetPossessedEnemy()->GetAIController()->Possess(GetPossessedEnemy());
		
		GetPossessedEnemy()->onStartDeathEvent.RemoveDynamic(this, &APawnParasite::OnPossessedEnemyDeath);

		UGameplayStatics::ApplyDamage(GetPossessedEnemy(), GetPossessedEnemy()->GetMaxHealth(), playerControllerRef, this, UDamageType_Environmental::StaticClass());
	}
	
	if(actorToPossess != nullptr)
	{
		actorToPossess->isPossessed = true;
		
		playerControllerRef->Possess(actorToPossess);

		//Set the move speed of possessed enemy to its chase speed
		actorToPossess->SetMoveSpeed(actorToPossess->GetChaseSpeed());

		actorToPossess->onStartDeathEvent.AddDynamic(this, &APawnParasite::OnPossessedEnemyDeath);
	}
	else
	{
		//If actorToPossess is null, that means we want to unpossess an enemy
		playerControllerRef->Possess(this);
	}
	
	possessedEnemy = actorToPossess;
}

void APawnParasite::OnPossessedEnemyDeath(APawnBase* enemy, AActor* causerActor)
{
	//Unpossess enemy upon death
	SetPossessed(nullptr);
}