// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseSkill.h"
#include "System\DataStructure\PlayerSkillRow.h"
#include "System\EdmundGameState.h"
ABaseSkill::ABaseSkill()
{
	PrimaryActorTick.bCanEverTick = false;
	bIsSkillActivate = false;
	SkillCurrentUpgradeIndex = 0;
	SkillMaxUpgradeIndex = 5;
	SkillName = "defaultSkillName";
}

// Called when the game starts or when spawned
void ABaseSkill::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseSkill::SelecteSkill()
{
	if (!bIsSkillActivate)
	{
		bIsSkillActivate = true;
		SelectedSkillActivate();
		SkillCurrentUpgradeIndex++;
	}
	else
	{
		if (SkillCurrentUpgradeIndex > SkillMaxUpgradeIndex)
		{
			UpgradeSkill();
			SkillCurrentUpgradeIndex++;
		}
	}
}

void ABaseSkill::SelectedSkillActivate()
{
}

void ABaseSkill::UpgradeSkill()
{
	//각 스킬에서 구현
}
