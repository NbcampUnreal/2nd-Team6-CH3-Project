// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TimerSkill.h"
#include "HealingPlant.generated.h"

class ABaseCharacter;

UCLASS()
class EDMUNDPRJ_API AHealingPlant : public ATimerSkill
{
	GENERATED_BODY()
public:
	AHealingPlant();
	void HealingCharacter();
	virtual void SpawnTimerSkill() override;
	virtual void Deactivate() override;
	virtual void UpgradeSkill() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<USphereComponent> CheckCharacterCollision = nullptr;
	FTimerHandle HealCycleHandle;
	FTimerHandle SpawnShakeHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float HealAmount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float HealCycleTime = 0;
};
