// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TimerSkill.h"
#include "Thunder.generated.h"


class AElectricEffect;
class UElectricEffectPool;

UCLASS()
class EDMUNDPRJ_API AThunder : public ATimerSkill
{
	GENERATED_BODY()
public:
	AThunder();
	virtual void BeginPlay() override;
	virtual void HitToMonster(TObjectPtr<ABaseMonster> Monster) override;
	virtual void Deactivate() override;
	virtual void SpawnTimerSkill() override;
public:
	TObjectPtr<UElectricEffectPool> ElectricEffectPool = nullptr;
};
