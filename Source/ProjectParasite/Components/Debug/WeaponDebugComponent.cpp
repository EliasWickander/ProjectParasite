// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDebugComponent.h"

#include "Components/CapsuleComponent.h"
#include "ProjectParasite/Actors/Weapons/MeleeWeapon.h"
#include "ProjectParasite/Pawns/PawnEnemy.h"

#include "ProjectParasite/Utilities/DevUtils.h"
#include "ProjectParasite/Utilities/DrawDebugHelpersExtended.h"

// Sets default values for this component's properties
UWeaponDebugComponent::UWeaponDebugComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
}


// Called when the game starts
void UWeaponDebugComponent::BeginPlay()
{
	Super::BeginPlay();

	weaponRef = Cast<AWeaponBase>(GetOwner());
}


// Called every frame
void UWeaponDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!debugEnabled)
		return;
}