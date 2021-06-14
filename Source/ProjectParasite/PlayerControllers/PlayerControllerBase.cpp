// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"

#include "EngineUtils.h"

void APlayerControllerBase::BeginPlay()
{	
	player = Cast<APawnParasite>(GetPawn());

	SetShowMouseCursor(true);

	EnableInput(this);
}

void APlayerControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	controlledPawn = Cast<APawnBase>(InPawn);

	if(controlledPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is possessing %s that doesn't seem to be derived from APawnBase"), *GetName(), *InPawn->GetName())
	}

	SetupInputBindings();
}

void APlayerControllerBase::SetupInputBindings()
{
	//Clear all bindings so that we can set completely new bindings every time we possess a pawn
	InputComponent->ClearActionBindings();

	SetupGeneralActions();
	
	if(Cast<APawnParasite>(controlledPawn) != nullptr)
	{
		//If controlled pawn is parasite
		SetupParasiteActions();
	}
	else if(Cast<APawnEnemy>(controlledPawn) != nullptr)
	{
		//If controlled pawn is enemy
		SetupEnemyActions();
	}
}

void APlayerControllerBase::SetupGeneralActions()
{
	InputComponent->BindAxis("MoveHorizontal", this, &APlayerControllerBase::MoveHorizontalInternal);
	InputComponent->BindAxis("MoveVertical", this, &APlayerControllerBase::MoveVerticalInternal);
}

void APlayerControllerBase::SetupParasiteActions()
{
	InputComponent->BindAction("Possess", EInputEvent::IE_Pressed, this, &APlayerControllerBase::DashInternal);
}

void APlayerControllerBase::SetupEnemyActions()
{
	InputComponent->BindAction("Unpossess", IE_Pressed, this, &APlayerControllerBase::UnpossessInternal);
	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerControllerBase::AttackInternal);
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
	
	if(controlledParasite->GetDashTimer() <= 0)
	{
		controlledParasite->Dash();
	}
}

void APlayerControllerBase::UnpossessInternal()
{
	APawnEnemy* controlledEnemy = Cast<APawnEnemy>(controlledPawn);
	
	controlledEnemy->SetPossessed(false);
}

void APlayerControllerBase::AttackInternal()
{
	APawnEnemy* controlledEnemy = Cast<APawnEnemy>(controlledPawn);
	
	controlledEnemy->Attack();
}
