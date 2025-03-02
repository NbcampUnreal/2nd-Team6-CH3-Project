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
	// ���� ���� �Լ�
	virtual void HitToMonster(TObjectPtr<ABaseMonster> Monster) override;
	void Attack();
	void ApplyDamage();
	TObjectPtr<ABaseMonster> FindCloseMonster();

	// ȸ��
	void RotationToTarget();

	virtual void UpgradeSkill() override;
	virtual void SpawnTimerSkill() override;
	virtual void Deactivate() override;
public:
	FTimerHandle ApplyDamageHandle;
	// ���� �ֱ� Ÿ�̸�
	FTimerHandle AttackCycleHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackSpeed = 0;

	// ȸ��
	FTimerHandle RotationHandle;
	TObjectPtr<ABaseMonster> TargetMonster;
};
