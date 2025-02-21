// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/AttackSkill.h"
#include "Mjolnir.generated.h"

class AElectricEffect;

UCLASS()
class EDMUNDPRJ_API AMjolnir : public AAttackSkill
{
	GENERATED_BODY()

public:
	AMjolnir();
	void CreateElectricEffect(int createCount);
	void ActivateElectricEffect(TObjectPtr<ABaseMonster> monster);
	TObjectPtr<AElectricEffect> FindDeactivateElectricEffect();
	virtual void HitToMonster(TObjectPtr<ABaseMonster> monster) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TSubclassOf<AElectricEffect> ElectricEffectClass = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TArray<TObjectPtr<AElectricEffect>> ElectricEffects = TArray<TObjectPtr<AElectricEffect>>();
};
