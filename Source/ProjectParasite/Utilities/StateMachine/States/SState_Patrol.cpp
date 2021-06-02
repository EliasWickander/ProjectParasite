// Fill out your copyright notice in the Description page of Project Settings.


#include "SState_Patrol.h"

SState_Patrol::SState_Patrol(AActor* caller) : State(caller)
{
}

void SState_Patrol::Start()
{
	UE_LOG(LogTemp, Warning, TEXT("Starting patrol"));
}

void SState_Patrol::Update()
{
	
}

void SState_Patrol::Exit()
{
	
}
