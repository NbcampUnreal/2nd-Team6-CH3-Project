// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/BaseMonster.h"
#include "SuicideMonster.generated.h"

/**
 * 
 */
UCLASS()
class EDMUNDPRJ_API ASuicideMonster : public ABaseMonster
{
	GENERATED_BODY()

public:
	void SetMonsterStatsByLevel() override;

	ASuicideMonster();

	void MonsterAttackCheck() override;

	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp, //동전
		AActor* OtherActor, //플레이어
		UPrimitiveComponent* OtherComp, //플레이어의 Collision(Capsule Component)
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void PlayParticle() override;
	void PlaySound() override;
};
