// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PassiveSkillManager.h"
#include "Player\BaseCharacter.h"
#include "Player\ElectricEffectPool.h"
// Sets default values for this component's properties
UPassiveSkillManager::UPassiveSkillManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPassiveSkillManager::BeginPlay()
{
	Super::BeginPlay();
	TObjectPtr<AActor> Owner = GetOwner();
	Character = Cast<ABaseCharacter>(Owner);
	TObjectPtr<UElectricEffectPool> electricEffectPool = Character->FindComponentByClass<UElectricEffectPool>();
	if (IsValid(electricEffectPool))
	{
		ElectricEffectPool = electricEffectPool;
	}
}

void UPassiveSkillManager::ActivatePassiveSkill(EPassiveSkillType passiveSkillType)
{
	if (Character != nullptr || !IsValid(Character))
	{
		TObjectPtr<AActor> Owner = GetOwner();
		Character = Cast<ABaseCharacter>(Owner);
	}
	PassiveSkillMap[passiveSkillType]++;
	switch (passiveSkillType)
	{
	case EPassiveSkillType::Null:
		break;
	case EPassiveSkillType::Berserker:
		//ĳ������ ��������Ʈ�� �����Ͽ� BerserkerSkill ���ε�
		break;
	case EPassiveSkillType::BloodAbsorbing:
		//ĳ������ ��������Ʈ�� �����Ͽ� BloodAbsorbing ���ε�
		break;
	case EPassiveSkillType::ElectricChain:
		break;
	case EPassiveSkillType::AmountMaxHp:
		break;
	case EPassiveSkillType::AmountMaxStamina:
		break;
	case EPassiveSkillType::AmountStaminaRecovery:
		break;
	default:
		break;
	}
}

//��������Ʈ
void UPassiveSkillManager::BerserkerSkill()
{
	float SkillLevel = PassiveSkillMap[EPassiveSkillType::Berserker];
	float HpRatio = Character->GetHP() / Character->MaxHP;
	float AttackMultiplier = 1.0f + (1.0f - HpRatio) * (MinHealthMultiplier - (1.0f - SkillLevel / 10));
	Character->AttackDamage *= AttackMultiplier;
}
//��������Ʈ
void UPassiveSkillManager::BloodAbsorbingSkill()
{
	float SkillLevel = PassiveSkillMap[EPassiveSkillType::BloodAbsorbing];
	Character->AmountHP((1 + SkillLevel / 5) * BloodAbsorbingAmount);
}

void UPassiveSkillManager::ElectricChainSkill(FVector MonsterLocation)
{
	float SkillLevel = PassiveSkillMap[EPassiveSkillType::ElectricChain];
	if (!IsValid(ElectricEffectPool))
	{
		TObjectPtr<UElectricEffectPool> electricEffectPool = Character->FindComponentByClass<UElectricEffectPool>();
		if (!IsValid(electricEffectPool)) return;
		ElectricEffectPool = electricEffectPool;
	}
	ElectricEffectPool->ActivateElectricEffect(MonsterLocation, SkillLevel * 2);
}

void UPassiveSkillManager::AmountMaxHpSkill()
{
	float SkillLevel = PassiveSkillMap[EPassiveSkillType::AmountMaxHp];
	Character->MaxHP += SkillLevel * 5;
}

void UPassiveSkillManager::AmountMaxStaminaSkill()
{
	float SkillLevel = PassiveSkillMap[EPassiveSkillType::AmountMaxStamina];
	Character->MaxStamina += SkillLevel * 5;
}

void UPassiveSkillManager::AmountStaminaRecoverySkill()
{
	float SkillLevel = PassiveSkillMap[EPassiveSkillType::AmountStaminaRecovery];
	Character->StaminaRecoveryAmount += SkillLevel * 2;
}
