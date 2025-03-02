// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/AttackSkill.h"
#include "Mjolnir.generated.h"

class AElectricEffect;
class UElectricEffectPool;

UCLASS()
class EDMUNDPRJ_API AMjolnir : public AAttackSkill
{
	GENERATED_BODY()

public:
	virtual void HitToMonster(TObjectPtr<ABaseMonster> monster) override;
	virtual void BeginPlay() override;
	virtual void UpgradeSkill() override;
public:
	TObjectPtr<UElectricEffectPool> ElectricEffectPool = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Electric Setting")
	int ElectricCount = 3;
};
