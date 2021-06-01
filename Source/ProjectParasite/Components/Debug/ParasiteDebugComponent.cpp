// Fill out your copyright notice in the Description page of Project Settings.


#include "ParasiteDebugComponent.h"

#include "ProjectParasite/Pawns/PawnParasite.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UParasiteDebugComponent::UParasiteDebugComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;

	parasiteRef = Cast<APawnParasite>(GetOwner());
	// ...
}


// Called when the game starts
void UParasiteDebugComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UParasiteDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!debugEnabled)
		return;
	
	DrawPossessionRadius();
	// ...
}

void UParasiteDebugComponent::DrawPossessionRadius()
{
	DrawDebugSphere(GetWorld(), parasiteRef->GetActorLocation(), parasiteRef->GetPossessRadius(), pSphereSegments, pSphereColor);
}


