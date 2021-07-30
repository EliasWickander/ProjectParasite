// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_State_Possess.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"
#include "ProjectParasite/PlayerControllers/PlayerControllerBase.h"
#include "ProjectParasite/AIControllers/AIControllerBase.h"
#include "ProjectParasite/Utilities/StateMachine/StateMachine.h"

void UPlayer_State_Possess::Start()
{
	playerRef = Cast<APawnParasite>(stateMachine->GetOwner());
	
	//While player is attached, remove its collision
	playerRef->GetCollider()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	playerRef->SetCanMove(false);

	if(possessedEnemy->GetAIController() != nullptr)
		possessedEnemy->GetAIController()->UnPossess();

	playerOriginPos = playerRef->GetActorLocation();
	detachTimer = 0;
	currentState = PossessState::PrePossess;
}

void UPlayer_State_Possess::Update()
{
	HandlePossessionLoop();
}

void UPlayer_State_Possess::Exit()
{
	playerRef->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	//playerRef->SetActorLocation(possessedEnemy->GetActorLocation() - possessedEnemy->GetActorForwardVector() * 200);

	playerRef->SetCanMove(true);
}

void UPlayer_State_Possess::HandlePossessionLoop()
{
	switch(currentState)
	{
	case PossessState::PrePossess:
		{
			FVector napeLocation = possessedEnemy->GetNapeComponent()->GetComponentLocation();
	
			if(FVector::Dist(playerRef->GetActorLocation(), napeLocation) > 1 && !instantPossession)
			{
				MoveToEnemyNape();

				if(possessedEnemy->GetIsPendingDeath())
				{
					stateMachine->SetState("State_Idle");	
				}
			}
			else
			{
				playerRef->SetActorLocation(napeLocation);
				playerRef->SetActorRotation(possessedEnemy->GetActorForwardVector().Rotation());

				//Parent nape to player to make sure player follows possessed enemy
				playerRef->AttachToComponent(possessedEnemy->GetNapeComponent(), FAttachmentTransformRules::KeepWorldTransform);

				playerRef->SetPossessed(possessedEnemy);
				currentState = PossessState::Possess;
			}
			break;
		}
	case PossessState::Possess:
		{
			//If player has control of parasite again, that means enemy is unpossessed. Get back to idle
			//TODO: Find better solution to this
			if(playerRef->IsPlayerControlled())
			{
				playerRef->GetCollider()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
				targetDetachPoint = playerRef->GetActorLocation() + playerRef->GetActorForwardVector() * playerRef->detachTargetDist;
				targetDetachPoint.Z = playerOriginPos.Z;

				currentState = PossessState::PostPossess;
			}
			break;
		}
	case PossessState::PostPossess:
		{
			stateMachine->SetState("State_Dash");
			// detachTimer = FMath::Clamp<float>(detachTimer += playerRef->detachLocationLerpSpeed * GetWorld()->GetDeltaSeconds(), 0, 1);
			//
			// FVector newLocation = FMath::Lerp(playerRef->GetActorLocation(), targetDetachPoint, detachTimer);
			//
			// bool locationSet = playerRef->SetActorLocation(newLocation, true);
			//
			// if(!locationSet || FVector::Dist(playerRef->GetActorLocation(), targetDetachPoint) <= 0.3f)
			// {
			// 	stateMachine->SetState("State_Dash");		
			// }
			
			break;
		}
	}
}

void UPlayer_State_Possess::MoveToEnemyNape()
{
	//Rotate and move player towards enemy nape
	FVector napeLocation = possessedEnemy->GetNapeComponent()->GetComponentLocation();

	//TODO: Use linear interpolation instead of exponential
	FVector lerpedPos = FMath::Lerp(playerRef->GetActorLocation(), napeLocation, playerRef->attachLocationLerpSpeed * playerRef->GetWorld()->DeltaTimeSeconds);

	FRotator lerpedRot = FMath::Lerp(playerRef->GetActorRotation(), possessedEnemy->GetActorForwardVector().Rotation(), playerRef->attachRotationLerpSpeed * playerRef->GetWorld()->DeltaTimeSeconds);
			
	playerRef->SetActorLocation(lerpedPos);
	playerRef->SetActorRotation(lerpedRot);
	
}

void UPlayer_State_Possess::SetPossessedEnemy(APawnEnemy* enemy, bool instant)
{
	instantPossession = instant;
	possessedEnemy = enemy;
	int t = 1;
}