// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Meteor.h"
#include "Player\TimerSkillSpawnManagerComponent.h"
#include "Monster\BaseMonster.h"
#include "Kismet\GameplayStatics.h"
AMeteor::AMeteor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMeteor::HitToMonster(TObjectPtr<ABaseMonster> Monster)
{
	UGameplayStatics::ApplyDamage(
		Monster,
		30.0f,
		nullptr,
		this,
		UDamageType::StaticClass()
	);
	Deactivate();
}

void AMeteor::Deactivate()
{
	Super::Deactivate();
}

void AMeteor::SpawnTimerSkill()
{
	Super::SpawnTimerSkill();
}

void AMeteor::Tick(float deltaTime)
{
	FVector NewLocation = GetActorLocation() - FVector(0, 0, MoveSpeed);
	SetActorLocation(NewLocation);
}
