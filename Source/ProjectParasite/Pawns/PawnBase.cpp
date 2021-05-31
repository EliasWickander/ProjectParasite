// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APawnBase::Move(FVector moveDir)
{
	AddActorLocalOffset(moveDir * moveSpeed * GetWorld()->GetDeltaSeconds());
}

