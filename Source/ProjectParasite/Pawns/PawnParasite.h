// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnParasite.generated.h"


class UParasiteDebugComponent;
class UParasiteStateMachine;
class APawnEnemy;
UCLASS()
class PROJECTPARASITE_API APawnParasite : public APawnBase
{
	GENERATED_BODY()

public:

	APawnParasite();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PossessClosestEnemyInRadius();

	APawnEnemy* GetPossessedEnemy();
	float GetPossessRadius();

protected:
	virtual void BeginPlay() override;

private:
	void Dash();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParasiteDebugComponent* parasiteDebugger = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParasiteStateMachine* stateMachine = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float possessRadius = 100;

	APawnEnemy* possessedEnemy = nullptr;
};
