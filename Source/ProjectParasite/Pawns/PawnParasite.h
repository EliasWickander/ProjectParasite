// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnParasite.generated.h"


class UParasiteDebugComponent;
class UStateMachine;
class APawnEnemy;
UCLASS()
class PROJECTPARASITE_API APawnParasite : public APawnBase
{
	GENERATED_BODY()

public:

	friend class UPlayer_State_Idle;
	friend class UPlayer_State_Dash;
	friend class UPlayer_State_Possess;
	
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

	UPROPERTY()
	UStateMachine* stateMachine = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession", meta = (AllowPrivateAccess = "true"))
	float possessRadius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession", meta = (AllowPrivateAccess = "true"))
	float attachLocationLerpSpeed = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession", meta = (AllowPrivateAccess = "true"))
	float attachRotationLerpSpeed = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float dashTime = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float dashSpeed = 500;

	UPlayer_State_Idle* idleState;
	UPlayer_State_Dash* dashState;
	UPlayer_State_Possess* possessState;
	
	APawnEnemy* possessedEnemy = nullptr;
};
