// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnEnemy.h"
#include "MeleePawnEnemy.generated.h"

UCLASS()
class PROJECTPARASITE_API AMeleePawnEnemy : public APawnEnemy
{
	GENERATED_BODY()

public:
	virtual void Attack() override;
};
