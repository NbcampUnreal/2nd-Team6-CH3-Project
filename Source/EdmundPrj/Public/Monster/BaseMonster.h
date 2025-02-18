// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "System/EnumSet.h"
#include "BaseMonster.generated.h"

class USphereComponent;
class UWidgetComponent;

UCLASS()
class EDMUNDPRJ_API ABaseMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseMonster();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Component")
	USphereComponent* MonsterAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	int32 MonsterLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterHP = 100 + (MonsterLevel * 50);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterMaxHP = 100 + (MonsterLevel * 50);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterAttackDamage = 10.0f + (MonsterLevel * 5.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterArmor = 5.0f + (MonsterLevel * 2.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterMoveSpeed = 43.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterAttackSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	FName MonsterType;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	EMonsterState MonsterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effects")
	UParticleSystem* AttackParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effects")
	UParticleSystem* TakeDamageParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effects")
	USoundBase* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effects")
	USoundBase* TakeDamageSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Animation")
	UAnimMontage* AttackAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Animation")
	UAnimMontage* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Item")
	float MonsterExpReward = 25.0f + (MonsterLevel * 20.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Item")
	float MonsterGoldReward = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Item")
	float MonsterHealKitProbability = 0.02f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Item")
	float MonsterGoldProbability = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|UI")
	UWidgetComponent* MonsterOverHeadWidget;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	void MonsterDead();
	void MonsterDestroy();
	void DropReward();
	// 아직 UI가 없음. MonsterOverHeadWidget 만들 때, 체력바 이름을 HealthBar로 할 것
	void UpdateMonsterOverHeadWidget();

	FTimerHandle DeadAnimTimerHandle;
};
