// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnShooter.h"
#include "Components/CapsuleComponent.h"

APawnShooter::APawnShooter()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *RootComponent->GetName());
	
}

void APawnShooter::BeginPlay()
{
	Super::BeginPlay();
}

void APawnShooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
