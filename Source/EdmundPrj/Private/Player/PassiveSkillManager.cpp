// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PassiveSkillManager.h"
#include "Player\BaseCharacter.h"
// Sets default values for this component's properties
UPassiveSkillManager::UPassiveSkillManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	TObjectPtr<AActor> Owner = GetOwner();
	Character = Cast<ABaseCharacter>(Owner);
	
	// ...
}

void UPassiveSkillManager::ActivatePassiveSkill(EPassiveSkillType passiveSkillType)
{
	if (Character != nullptr || !IsValid(Character))
	{
		TObjectPtr<AActor> Owner = GetOwner();
		Character = Cast<ABaseCharacter>(Owner);
	}
	switch (passiveSkillType)
	{
	case EPassiveSkillType::Null:
		break;
	case EPassiveSkillType::Berserker:
		PassiveSkillMap[passiveSkillType]++;
		
		break;
	case EPassiveSkillType::BloodAbsorbing:
		break;
	}
}

//µ¨¸®°ÔÀÌÆ®
void UPassiveSkillManager::BerserkerSkill()
{
	if (PassiveSkillMap[EPassiveSkillType::Berserker] > 0)
	{
		float SkillLevel = PassiveSkillMap[EPassiveSkillType::Berserker];
		float HpRatio = Character->GetHP() / Character->MaxHP;
		float AttackMultiplier = 1.0f + (1.0f - HpRatio) * (MinHealthMultiplier - (1.0f - SkillLevel / 10));
		Character->AttackDamage *= AttackMultiplier;
	}
}


// Called when the game starts
void UPassiveSkillManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPassiveSkillManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

