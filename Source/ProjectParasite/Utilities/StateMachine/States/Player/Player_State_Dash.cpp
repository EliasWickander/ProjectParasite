// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_State_Dash.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Utilities/StateMachine/StateMachine.h"


void UPlayer_State_Dash::Start()
{
	playerRef = Cast<APawnParasite>(stateMachine->GetOwner());
	
	dashTimer = 0;

	//Set dash direction to go forward
	dashDir = playerRef->GetActorForwardVector();
	dashDir.Normalize();

	//Store reference to move speed pre-dash
	prevMoveSpeed = playerRef->GetMoveSpeed();

	//Disable movement input during dash
	playerRef->SetCanMove(false);

	playerRef->SetMoveSpeed(playerRef->dashSpeed);

	playerRef->SetActorRotation(dashDir.Rotation());
}

void UPlayer_State_Dash::Update()
{	
	//Dash in {dashTime} seconds
	if(dashTimer < playerRef->dashTime)
	{
		dashTimer += playerRef->GetWorld()->DeltaTimeSeconds;

		playerRef->AddMovementInput(dashDir);	

		//if there are enemies close enough, possess
		playerRef->PossessClosestEnemyInRadius();
	}
	else
	{	
		//When finished dashing, transition back to idle
		stateMachine->SetState("State_Idle");
	}
}

void UPlayer_State_Dash::Exit()
{
	//Reset move speed to what it was before dash
	playerRef->SetMoveSpeed(prevMoveSpeed);
	playerRef->SetCanMove(true);
	playerRef->SetDashTimer(playerRef->GetDashCooldown());
}
