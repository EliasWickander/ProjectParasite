// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeWeapon.h"
#include "Knife.generated.h"

UCLASS()
class PROJECTPARASITE_API AKnife : public AMeleeWeapon
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void Use() override;
};
