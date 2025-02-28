// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActiveSkillSpawnManager.h"
#include "Player\AttackSkill.h"

// Sets default values for this component's properties
UActiveSkillSpawnManager::UActiveSkillSpawnManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActiveSkillSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UActiveSkillSpawnManager::ActivateProbCalculate()
{
	if (ActivateSkillList.IsEmpty()) return;
	for (EActiveSkillType skillType : ActivateSkillList)
	{
		int RandomInt = FMath::RandRange(1, 100);
		if (RandomInt > ActivateProb)
		{
			ActivateActiveSkill(skillType);
		}

	}
}

void UActiveSkillSpawnManager::CreateActiveSkill(TSubclassOf<AAttackSkill> attackSkill, EActiveSkillType skillType, int createCount)
{
	if (!attackSkill) return;
	for (int i = 0; i < createCount; i++)
	{
		if (!IsValid(GetWorld())) return;
		TObjectPtr<AAttackSkill> skill = GetWorld()->SpawnActor<AAttackSkill>(attackSkill);

		if (!skill) continue;
		skill->SetActorHiddenInGame(true);
		skill->SetActorEnableCollision(false);
		skill->SetActorTickEnabled(false);
		ActiveSkillMap.FindOrAdd(skillType).Add(skill);
	}
}

void UActiveSkillSpawnManager::ActivateActiveSkill(EActiveSkillType skillType)
{
	TWeakObjectPtr<AAttackSkill> skill = nullptr;
	skill = FindDeactivateActiveSkill(skillType);
	if (!skill.IsValid())
	{
		CreateActiveSkill(ActiveSkillClassMap[skillType], skillType, 10);
		skill = FindDeactivateActiveSkill(skillType);
	}
	if (!skill.IsValid()) return;

	FVector skillLocation = GetOwner()->GetActorLocation();

	skill->FinishPos = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * skill->SkillRange);
	skill->SetActorLocation(skillLocation);
	skill->SetActorEnableCollision(true);
	skill->SetActorHiddenInGame(false);
	skill->SetActorTickEnabled(true);
}

TObjectPtr<AAttackSkill> UActiveSkillSpawnManager::FindDeactivateActiveSkill(EActiveSkillType skillType)
{
	if (!ActiveSkillMap.Find(skillType) || ActiveSkillMap[skillType].Num() == 0) return nullptr;
	for (TObjectPtr<AAttackSkill>& skill : ActiveSkillMap[skillType])
	{
		if (skill->IsHidden())
		{
			return skill;
		}
	}
	return nullptr;
}

