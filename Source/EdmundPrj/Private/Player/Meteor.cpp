// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Meteor.h"
#include "Player\TimerSkillSpawnManagerComponent.h"
AMeteor::AMeteor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMeteor::HitToMonster(TObjectPtr<ABaseMonster> Monster)
{
	Deactivate();
}

void AMeteor::Deactivate()
{
	TimerSkillSpanwManager->DeactivateTimerSkill(this);
}

void AMeteor::Tick(float deltaTime)
{
	FVector NewLocation = GetActorLocation() - FVector(0, 0, MoveSpeed);
	SetActorLocation(NewLocation);
}
