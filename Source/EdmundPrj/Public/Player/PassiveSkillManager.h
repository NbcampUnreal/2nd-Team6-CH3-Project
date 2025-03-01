// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System\EnumSet.h"
#include "Components/ActorComponent.h"
#include "PassiveSkillManager.generated.h"

class ABaseCharacter;
class UElectricEffectPool;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDMUNDPRJ_API UPassiveSkillManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPassiveSkillManager();
	virtual void BeginPlay() override;
	void ActivatePassiveSkill(EPassiveSkillType passiveSkillType);
	void BerserkerSkill();
	void BloodAbsorbingSkill();
	void ElectricChainSkill(FVector MonsterLocation);
	void AmountMaxHpSkill();
	void AmountMaxStaminaSkill();
	void AmountStaminaRecoverySkill();
public:	

	TObjectPtr<ABaseCharacter> Character;
	TMap<EPassiveSkillType, int> PassiveSkillMap;
	TObjectPtr<UElectricEffectPool> ElectricEffectPool = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BloodAbsorbing")
	int BloodAbsorbingAmount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Berserker")
	float MinHealthMultiplier = 0;
};
