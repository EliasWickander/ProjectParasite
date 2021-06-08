// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_State_Dash.h"
#include "ProjectParasite/Pawns/PawnParasite.h"

UPlayer_State_Dash::UPlayer_State_Dash()
{
	controller = Cast<APawnParasite>(GetOwner());
}

void UPlayer_State_Dash::Start()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Dash"));

	UE_LOG(LogTemp, Warning, TEXT("%f"), controller->dashTime);
	
	timer = 0;
	
	dashDir = controller->GetActorForwardVector();
	dashDir.Normalize();

	prevMoveSpeed = controller->GetMoveSpeed();

	//Prevent player from moving during dash
	controller->SetCanMove(false);
	controller->SetMoveSpeed(controller->dashSpeed);
}

void UPlayer_State_Dash::Update()
{
	
	if(timer < controller->dashTime)
	{
		timer += GetWorld()->DeltaTimeSeconds;
		
		controller->AddMovementInput(dashDir);	

		//if there are enemies close enough, possess
		controller->PossessClosestEnemyInRadius();
	}
	else
	{
		//When been in this state for more than certain amount of time, transition back to idle
		OnStateTransition.Broadcast("State_Idle");
	}
}

void UPlayer_State_Dash::Exit()
{
	//Reset move speed to what it was before dash
	controller->SetMoveSpeed(prevMoveSpeed);
	controller->SetCanMove(true);
}
