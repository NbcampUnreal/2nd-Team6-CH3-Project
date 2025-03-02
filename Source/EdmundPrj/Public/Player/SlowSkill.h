// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/AttackSkill.h"
#include "SlowSkill.generated.h"

/**
 * 
 */
UCLASS()
class EDMUNDPRJ_API ASlowSkill : public AAttackSkill
{
	GENERATED_BODY()
public:
	virtual void HitToMonster(TObjectPtr<ABaseMonster> monster);
	virtual void UpgradeSkill();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float SlowTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float SlowApplicationRate = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float AmountSlowApplicationRate = 0;

	FTimerHandle SlowHandle;
};
