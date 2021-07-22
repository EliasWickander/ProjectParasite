// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/Actors/Weapons/WeaponBase.h"

#include "ProjectParasite/GameStateCustom.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectParasite/GameManager.h"


void APlayerControllerBase::BeginPlay()
{	
	playerRef = Cast<APawnParasite>(GetPawn());
	controlledPawn = playerRef;
	gameStateRef = Cast<AGameStateCustom>(UGameplayStatics::GetGameState(GetWorld()));
	gameManagerRef = Cast<UGameManager>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	SetShowMouseCursor(true);

	EnableInput(this);
	
	SetupInputBindings();

	gameManagerRef->OnPauseGameEvent.AddDynamic(this, &APlayerControllerBase::OnGamePaused);
	gameManagerRef->OnUnpauseGameEvent.AddDynamic(this, &APlayerControllerBase::OnGameUnpaused);
}

void APlayerControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	controlledPawn = Cast<APawnBase>(InPawn);
}

void APlayerControllerBase::SetupInputBindings()
{
	SetupGeneralActions();
	SetupParasiteActions();
	SetupEnemyActions();	
}

void APlayerControllerBase::SetupGeneralActions()
{
	InputComponent->BindAxis("MoveHorizontal", this, &APlayerControllerBase::MoveHorizontalInternal);
	InputComponent->BindAxis("MoveVertical", this, &APlayerControllerBase::MoveVerticalInternal);
}

void APlayerControllerBase::SetupParasiteActions()
{
	InputComponent->BindAction("Possess", EInputEvent::IE_Pressed, this, &APlayerControllerBase::PossessInternal);
}

void APlayerControllerBase::SetupEnemyActions()
{
	InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerControllerBase::AttackInternal);
}

void APlayerControllerBase::MoveHorizontalInternal(float axis)
{
	controlledPawn->MoveHorizontal(axis);	
}

void APlayerControllerBase::MoveVerticalInternal(float axis)
{
	controlledPawn->MoveVertical(axis);	
}

void APlayerControllerBase::DashInternal()
{
	APawnParasite* controlledParasite = Cast<APawnParasite>(controlledPawn);

	if(controlledParasite != nullptr)
	{
		//Dash if not on cooldown
		if(controlledParasite->GetDashTimer() <= 0)
		{
			controlledParasite->Dash();
		}	
	}
}

void APlayerControllerBase::PossessInternal()
{
	//If already possessing enemy, unpossess
	if(playerRef->GetPossessedEnemy())
	{
		playerRef->SetPossessed(nullptr);	
	}
	else
	{
		DashInternal();
	}
}

void APlayerControllerBase::AttackInternal()
{
	APawnEnemy* controlledEnemy = Cast<APawnEnemy>(controlledPawn);
	
	if(controlledEnemy != nullptr)
	{
		if(controlledEnemy->GetWeapon())
		{
			controlledEnemy->GetWeapon()->Use();
		}
	}		
}

void APlayerControllerBase::OnGamePaused()
{
	DisableInput(this);
}

void APlayerControllerBase::OnGameUnpaused()
{
	EnableInput(this);
}
