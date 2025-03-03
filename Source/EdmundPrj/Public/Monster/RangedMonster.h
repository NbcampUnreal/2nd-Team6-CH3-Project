// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/BaseMonster.h"
#include "UObject/Class.h"
#include "RangedMonster.generated.h"

class AMonsterSpawner;
class AMonsterBulletPool;

UCLASS()
class EDMUNDPRJ_API ARangedMonster : public ABaseMonster
{
	GENERATED_BODY()

public:

	void SetMonsterStatsByLevel() override;

	ARangedMonster();

	void MonsterAttackCheck() override;
	void Fire();

	void PlayParticle() override;
	void PlaySound() override;

protected:
	virtual void BeginPlay() override;
};
