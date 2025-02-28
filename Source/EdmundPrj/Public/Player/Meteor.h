// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TimerSkill.h"
#include "Meteor.generated.h"

class UParticleSystem;


UCLASS()
class EDMUNDPRJ_API AMeteor : public ATimerSkill
{
	GENERATED_BODY()
	AMeteor();
	virtual void HitToMonster(TObjectPtr<ABaseMonster> Monster) override;
	void Deactivate();
	virtual void SpawnTimerSkill() override;
	virtual void Tick(float deltaTime);
public:

};
