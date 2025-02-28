// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SkillManager.h"

// Sets default values for this component's properties
USkillManager::USkillManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USkillManager::ActivateSkill(int SkillIndex)
{
	//for(TObjectPtr<ABaseSkill>& )
}


// Called when the game starts
void USkillManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

