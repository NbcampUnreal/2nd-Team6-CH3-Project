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
	virtual void UpgradeSkill() override;
public:
	TObjectPtr<UElectricEffectPool> ElectricEffectPool = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Electric Setting")
	int ElectricCount = 3;
};
