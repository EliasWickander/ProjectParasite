// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Handle attack timer
	attackTimer = FMath::Clamp<float>(attackTimer - DeltaTime, 0, attackRate);
}
