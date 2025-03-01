#include "Player/SkillManager.h"
#include "System\DataStructure\PlayerSkillRow.h"
#include "System\EnumSet.h"
#include "Player\BaseSkill.h"
#include "Player\TimerSkillSpawnManagerComponent.h"
#include "Player\ActiveSkillSpawnManager.h"

USkillManager::USkillManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USkillManager::ActivateSkill(FPlayerSkillRow& row)
{
	switch (row.SkillType)
	{
	case ESkillType::ActiveSkill:
		ActivateActiveSkill(row.ActiveSkillType);
		break;
	case ESkillType::PassiveSkill:
		ActivatePassiveSkill(row.PassiveSkillType);
		break;
	case ESkillType::TimerSkill:
		ActivateTimerSkill(row.TimerSkillType);
		break;
	}
}

void USkillManager::ActivateTimerSkill(ETimerSkillType SkillType)
{
	if (TimerSkillManager == nullptr)
	{
		TimerSkillManager = GetOwner()->FindComponentByClass<UTimerSkillSpawnManagerComponent>();
	}

	TimerSkillManager->SetSkillTimer(SkillType);
}

void USkillManager::ActivateActiveSkill(EActiveSkillType SkillType)
{
	if (ActiveSkillManager == nullptr)
	{
		ActiveSkillManager = GetOwner()->FindComponentByClass<UActiveSkillSpawnManager>();
	}
	ActiveSkillManager->ActivateSkillList.Add(SkillType);
}

void USkillManager::ActivatePassiveSkill(EPassiveSkillType SkillType)
{
	switch (SkillType)
	{
	case EPassiveSkillType::Berserker:
		break;
	default:
		break;
	}
}

// Called when the game starts
void USkillManager::BeginPlay()
{
	Super::BeginPlay();
	TimerSkillManager = GetOwner()->FindComponentByClass<UTimerSkillSpawnManagerComponent>();
	ActiveSkillManager = GetOwner()->FindComponentByClass<UActiveSkillSpawnManager>();
	// ...
	
}

