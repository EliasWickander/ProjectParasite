// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnParasite.h"

#include <Actor.h>

#include "ProjectParasite/AIControllers/AIControllerBase.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/Components/Debug/ParasiteDebugComponent.h"
#include "ProjectParasite/DamageTypes/DamageType_Environmental.h"
#include "ProjectParasite/Utilities/StateMachine/StateMachine.h"
#include "ProjectParasite/Utilities/StateMachine/States/Player/Player_State_Idle.h"
#include "ProjectParasite/Utilities/StateMachine/States/Player/Player_State_Dash.h"
#include "ProjectParasite/Utilities/StateMachine/States/Player/Player_State_Possess.h"

#include "ProjectParasite/PlayerControllers/PlayerControllerBase.h"
#include "ProjectParasite/GameModes/EliminationGamemode.h"

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

	gameModeRef = Cast<AEliminationGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	SetMoveSpeed(baseMoveSpeed);
	stateMachine->SetState("State_Idle");
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

 		FVector dirToEnemy = enemy->GetActorLocation() - GetActorLocation();
 		dirToEnemy.Z = 0;
 		
 		if(dirToEnemy.Size() <= possessRadius)
 		{
 			dirToEnemy.Normalize();
 			float dotProduct = FVector::DotProduct(dirToEnemy, enemy->GetActorForwardVector());

 			float angle = FMath::RadiansToDegrees(acos(dotProduct));

 			if(angle <= possessAngle || enemy->GetAIController()->GetCurrentState() == EnemyStates::State_Stunned)
 			{
 				enemiesInRadius.Add(enemyFound);		
 			}
 		}
	}

	//If there are enemies in radius, possess the first one
	if(enemiesInRadius.Num() <= 0)
		return;

	//Save reference to enemy player is possessing

	possessState->SetPossessedEnemy(enemiesInRadius[0]);
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

		GetPossessedEnemy()->GetCollider()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
		
		GetPossessedEnemy()->onStartDeathEvent.RemoveDynamic(this, &APawnParasite::OnPossessedEnemyDeath);

		UGameplayStatics::ApplyDamage(GetPossessedEnemy(), GetPossessedEnemy()->GetMaxHealth(), playerControllerRef, this, UDamageType_Environmental::StaticClass());
	}
	
	if(actorToPossess != nullptr)
	{
		actorToPossess->isPossessed = true;
		actorToPossess->OnPossessed();
		
		actorToPossess->GetAIController()->SetCurrentState(EnemyStates::State_Idle);
		playerControllerRef->Possess(actorToPossess);

		//Set the move speed of possessed enemy to its chase speed
		actorToPossess->SetMoveSpeed(actorToPossess->GetPossessedSpeed());

		actorToPossess->GetCollider()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);
		actorToPossess->onStartDeathEvent.AddDynamic(this, &APawnParasite::OnPossessedEnemyDeath);

		gameModeRef->RemoveFromEnemiesAlive(actorToPossess);

		gameModeRef->HandleWinCondition();
		OnPossessedEnemy(actorToPossess);
	}
	else
	{
		//If actorToPossess is null, that means we want to unpossess an enemy
		playerControllerRef->Possess(this);
	}
	
	possessedEnemy = actorToPossess;
}

bool APawnParasite::IsPlayerDashing()
{
	return stateMachine->currentState == dashState;
}

bool APawnParasite::IsPlayerIdle()
{
	return stateMachine->currentState == idleState;	
}

bool APawnParasite::IsPlayerPossessing()
{
	return stateMachine->currentState == possessState;
}

void APawnParasite::OnPossessedEnemyDeath(APawnBase* enemy, AActor* causerActor)
{
	//Unpossess enemy upon death
	SetPossessed(nullptr);
}
