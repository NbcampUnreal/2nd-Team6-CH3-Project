// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PassiveSkillManager.h"
#include "Player\BaseCharacter.h"
#include "Player\ElectricEffectPool.h"
#include "System\EdmundGameState.h"
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
	PassiveSkillMap.FindOrAdd(passiveSkillType);

	++PassiveSkillMap[passiveSkillType];
	
	switch (passiveSkillType)
	{
	case EPassiveSkillType::Null:
		break;
	case EPassiveSkillType::Berserker:
		if (PassiveSkillMap[passiveSkillType] == 1)
		{
			Character->OnBerserkerSkillActivate.AddDynamic(this, &UPassiveSkillManager::BerserkerSkill);
		}
		break;
	case EPassiveSkillType::BloodAbsorbing:
		if (PassiveSkillMap[passiveSkillType] == 1)
		{
			Character->OnBloodAbsorbingSkillActivate.AddDynamic(this, &UPassiveSkillManager::BloodAbsorbingSkill);
		}
		break;
	case EPassiveSkillType::ElectricChain:
		if (PassiveSkillMap[passiveSkillType] == 1)
		{
			Character->OnElectricChainSkillActivate.AddDynamic(this, &UPassiveSkillManager::ElectricChainSkill);
		}
		break;
	case EPassiveSkillType::AmountMaxHp:
		AmountMaxHpSkill();
		break;
	case EPassiveSkillType::AmountMaxStamina:
		AmountMaxStaminaSkill();
		break;
	case EPassiveSkillType::AmountStaminaRecovery:
		AmountStaminaRecoverySkill();
		break;
	default:
		break;
	}
}

//µ¨¸®°ÔÀÌÆ®
void UPassiveSkillManager::BerserkerSkill()
{
	float SkillLevel = PassiveSkillMap[EPassiveSkillType::Berserker];
	float HpRatio = Character->GetHP() / Character->MaxHP;
	float AttackMultiplier = 1.0f + (1.0f - HpRatio) * (MinHealthMultiplier - (1.0f - SkillLevel / 10));
	Character->AttackDamage *= AttackMultiplier;
}
//µ¨¸®°ÔÀÌÆ®
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
	Character->MaxHP += SkillLevel * 20;
	AEdmundGameState* CurrentGameState = Cast<AEdmundGameState>(GetWorld()->GetGameState());
	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerHp(Character->MaxHP, Character->GetHP());
	}
}

void UPassiveSkillManager::AmountMaxStaminaSkill()
{
	float SkillLevel = PassiveSkillMap[EPassiveSkillType::AmountMaxStamina];
	Character->MaxStamina += SkillLevel * 20;
	AEdmundGameState* CurrentGameState = Cast<AEdmundGameState>(GetWorld()->GetGameState());
	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerOther(Character->MaxStamina, Character->Stamina);
	}
}

void UPassiveSkillManager::AmountStaminaRecoverySkill()
{
	float SkillLevel = PassiveSkillMap[EPassiveSkillType::AmountStaminaRecovery];
	Character->StaminaRecoveryAmount += SkillLevel * 2;
}
