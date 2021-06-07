// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PawnBase.h"
#include "PawnEnemy.generated.h"

class AAIController;
class APawnParasite;
class UWeapon;

UCLASS()
class PROJECTPARASITE_API APawnEnemy : public APawnBase
{
	GENERATED_BODY()

public:

	APawnEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Attack();

	virtual void SetPossessed(bool isPossessed);
	USceneComponent* GetNapeComponent() { return napeComponent; }
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* napeComponent = nullptr;

	APawnParasite* playerRef = nullptr;

private:
	void Unpossess();
	APawnParasite* FindPlayerInWorld();

	AAIController* AIController = nullptr;
};
