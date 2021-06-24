// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"

#include "EngineUtils.h"

void APlayerControllerBase::BeginPlay()
{	
	playerRef = Cast<APawnParasite>(GetPawn());

	SetShowMouseCursor(true);

	EnableInput(this);

	controlledPawn = Cast<APawnBase>(GetPawn());

	if(controlledPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is possessing %s that doesn't seem to be derived from APawnBase"), *GetName(), *GetPawn()->GetName())
	}

	SetupInputBindings();
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
}

void APlayerControllerBase::SetupInputBindings()
{
	//Clear all bindings so that we can set completely new bindings every time we possess a pawn
	//InputComponent->ClearActionBindings();

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

	if(controlledParasite != nullptr)
	{
		if(controlledParasite->GetDashTimer() <= 0)
		{
			controlledParasite->Dash();
		}	
	}
}

void APlayerControllerBase::UnpossessInternal()
{
	playerRef->SetPossessed(nullptr);
}

void APlayerControllerBase::AttackInternal()
{
	APawnEnemy* controlledEnemy = Cast<APawnEnemy>(controlledPawn);

	if(controlledEnemy != nullptr)
	{
		controlledEnemy->Attack();	
	}
}
