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

friend class UPlayer_State_Idle;
friend class UPlayer_State_Dash;
friend class UPlayer_State_Possess;
	
public:
	
	APawnParasite();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Possession")
	void Dash();

	UFUNCTION(BlueprintCallable, Category = "Possession")
	void PossessClosestEnemyInRadius();

	UFUNCTION(BlueprintCallable, Category = "Possession")
	APawnEnemy* GetPossessedEnemy() { return possessedEnemy; }
	float GetPossessRadius() { return possessRadius; }

	float GetDashCooldown() { return dashCooldown; }
	float GetDashTimer() { return dashTimer; }
	float GetBaseMoveSpeed() { return baseMoveSpeed; }
	void SetDashTimer(float value) { dashTimer = value; }

	void SetPossessed(APawnEnemy* actorToPossess);

	float GetKillScore() { return killScore; }

protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParasiteDebugComponent* parasiteDebugger = nullptr;

	UPROPERTY()
	UStateMachine* stateMachine = nullptr;

	UFUNCTION()
	void OnPossessedEnemyDeath(APawnBase* enemy, AActor* causerActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float baseMoveSpeed = 300;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession", meta = (AllowPrivateAccess = "true"))
	float possessRadius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession", meta = (AllowPrivateAccess = "true"))
	float attachLocationLerpSpeed = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession", meta = (AllowPrivateAccess = "true"))
	float attachRotationLerpSpeed = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession", meta = (AllowPrivateAccess = "true"))
	float detachLocationLerpSpeed = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession", meta = (AllowPrivateAccess = "true"))
	float detachTargetDist = 200;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score", meta = (AllowPrivateAccess = "true"))
	float killScore = 400;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float dashTime = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float dashCooldown = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float dashSpeed = 500;

	float dashTimer = 0;
	
	UPlayer_State_Idle* idleState;
	UPlayer_State_Dash* dashState;
	UPlayer_State_Possess* possessState;
	
	APawnEnemy* possessedEnemy = nullptr;
};
