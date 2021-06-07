// Fill out your copyright notice in the Description page of Project Settings.


#include "ParasiteDebugComponent.h"

#include "ProjectParasite/Pawns/PawnParasite.h"
#include "DrawDebugHelpers.h"

UParasiteDebugComponent::UParasiteDebugComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;

	parasiteRef = Cast<APawnParasite>(GetOwner());
}

void UParasiteDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!debugEnabled)
		return;
	
	DrawPossessionRadius();
}

void UParasiteDebugComponent::DrawPossessionRadius()
{
	DrawDebugSphere(GetWorld(), parasiteRef->GetActorLocation(), parasiteRef->GetPossessRadius(), pSphereSegments, pSphereColor);
}


