// Fill out your copyright notice in the Description page of Project Settings.


#include "SState_Idle.h"

SState_Idle::SState_Idle(AActor* caller) : State(caller)
{
}

void SState_Idle::Start()
{
	UE_LOG(LogTemp, Warning, TEXT("Starting idle"));
}

void SState_Idle::Update()
{
}

void SState_Idle::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("exiting idle"));
}
