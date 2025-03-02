// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/AttackSkill.h"
#include "Freezing.generated.h"

/**
 * 
 */
UCLASS()
class EDMUNDPRJ_API AFreezing : public AAttackSkill
{
	GENERATED_BODY()
public:
	void CreateFrozenEffect(int createCount);
	TObjectPtr<AActor> ActivateFrozenEffect(TObjectPtr<ABaseMonster> monster);
	TObjectPtr<AActor> FindDeactivateFrozenEffect();
	virtual void HitToMonster(TObjectPtr<ABaseMonster> monster) override;
	virtual void UpgradeSkill() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float FrozenTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TSubclassOf<AActor> FrozenEffectClass = nullptr;
	TArray<TObjectPtr<AActor>> FrozenEffects = TArray<TObjectPtr<AActor>>();
};