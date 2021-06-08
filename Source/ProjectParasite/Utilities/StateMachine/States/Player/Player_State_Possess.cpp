// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_State_Possess.h"

#include "Components/CapsuleComponent.h"
#include "ProjectParasite/Pawns/PawnParasite.h"
#include "ProjectParasite/PlayerControllers/PlayerControllerParasite.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"

UPlayer_State_Possess::UPlayer_State_Possess()
{
	controller = Cast<APawnParasite>(GetOwner());
}

void UPlayer_State_Possess::Start()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Possess"));

	possessedEnemy = controller->GetPossessedEnemy();

	//While player is attached, remove its collision
	controller->GetCollider()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	controller->SetCanMove(false);
	possessedEnemy->SetPossessed(true);

	currentState = PossessState::PrePossess;
}

void UPlayer_State_Possess::Update()
{
	HandlePossessionLoop();
}

void UPlayer_State_Possess::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("Exit Possess"));

	controller->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	controller->SetActorLocation(possessedEnemy->GetActorLocation() + -possessedEnemy->GetActorForwardVector() * 200);

	controller->GetCollider()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	controller->SetCanMove(true);
}

void UPlayer_State_Possess::HandlePossessionLoop()
{
	switch(currentState)
	{
	case PossessState::PrePossess:
		{
			FVector napeLocation = possessedEnemy->GetNapeComponent()->GetComponentLocation();
	
			if(FVector::Dist(controller->GetActorLocation(), napeLocation) > 1)
			{
				MoveToEnemyNape();
			}
			else
			{
				controller->SetActorLocation(napeLocation);
				controller->SetActorRotation(possessedEnemy->GetActorForwardVector().Rotation());

				//Parent nape to player to make sure player follows possessed enemy
				controller->AttachToComponent(possessedEnemy->GetNapeComponent(), FAttachmentTransformRules::KeepWorldTransform);
		
				controller->playerControllerRef->Possess(possessedEnemy);
				currentState = PossessState::Possess;
			}
			break;
		}
	case PossessState::Possess:
		{
			//If player has control of parasite again, that means enemy is unpossessed. Get back to idle
			//TODO: Find better solution to this
			if(controller->IsPlayerControlled())
			{
				currentState = PossessState::PostPossess;
			}
			break;
		}
	case PossessState::PostPossess:
		{
			OnStateTransition.Broadcast("State_Idle");
			break;
		}
	}
}

void UPlayer_State_Possess::MoveToEnemyNape()
{
	//Rotate and move player towards enemy nape
	FVector napeLocation = possessedEnemy->GetNapeComponent()->GetComponentLocation();

	//TODO: Use linear interpolation instead of exponential
	FVector lerpedPos = FMath::Lerp(controller->GetActorLocation(), napeLocation, controller->attachLocationLerpSpeed * GetWorld()->DeltaTimeSeconds);

	FRotator lerpedRot = FMath::Lerp(controller->GetActorRotation(), possessedEnemy->GetActorForwardVector().Rotation(), controller->attachRotationLerpSpeed * GetWorld()->DeltaTimeSeconds);
			
	controller->SetActorLocation(lerpedPos);
	controller->SetActorRotation(lerpedRot);
	
}
