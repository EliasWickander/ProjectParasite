// Fill out your copyright notice in the Description page of Project Settings.


#include "P_State_Dash.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/PlayerControllers/ParasitePlayerController.h"

UP_State_Dash::UP_State_Dash()
{
	controller = Cast<APawnParasite>(GetOwner());
}

void UP_State_Dash::Start()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Dash"));

	timer = 0;
	
	if(dashInRotationDir)
	{
		dashDir = controller->GetActorForwardVector();
	}
	else
	{
		//Dash in direction of input
	}

	dashDir.Normalize();

	controller->playerControllerRef->SetPlayerInputEnabled(false);
}

void UP_State_Dash::Update()
{
	if(timer < dashTime)
	{
		timer += GetWorld()->DeltaTimeSeconds;
		
		controller->AddMovementInput(dashDir);	
	}
	else
	{
		OnStateTransition.Broadcast("State_Idle");
	}
}

void UP_State_Dash::Exit()
{
}
