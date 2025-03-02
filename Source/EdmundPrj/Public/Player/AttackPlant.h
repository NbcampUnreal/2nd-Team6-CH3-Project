// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Plant.h"
#include "AttackPlant.generated.h"

/**
 * 
 */
UCLASS()
class EDMUNDPRJ_API AAttackPlant : public APlant
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	// 공격 관련 함수
	virtual void HitToMonster(TObjectPtr<ABaseMonster> Monster) override;
	void Attack();
	void ApplyDamage();
	TObjectPtr<ABaseMonster> FindCloseMonster();

	// 회전
	void RotationToTarget();

	virtual void UpgradeSkill() override;
	virtual void SpawnTimerSkill() override;
	virtual void Deactivate() override;
public:
	FTimerHandle ApplyDamageHandle;
	// 공격 주기 타이머
	FTimerHandle AttackCycleHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackSpeed = 0;

	// 회전
	FTimerHandle RotationHandle;
	TObjectPtr<ABaseMonster> TargetMonster;
};
