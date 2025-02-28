// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "System/EnumSet.h"
#include "Components/AudioComponent.h"
#include "BaseMonster.generated.h"

class AMonsterSpawner;
class USphereComponent;
class UWidgetComponent;
class ABaseItem;

UCLASS()
class EDMUNDPRJ_API ABaseMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseMonster();

float GetMonsterAttackDamage();

virtual void MonsterAttackCheck();

virtual void MonsterDead();

void SetIsDead(bool bNewIsDead);

void SetCanDropReward(bool NewState);

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Sound")
UAudioComponent* CurrentAudioComp;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|BulletPool")
AMonsterSpawner* MonsterSpawner;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Component")
USphereComponent* MonsterAttackRange;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Spawn")
TArray<TSubclassOf<ABaseItem>> AllItems;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Spawn")
TSubclassOf<ABaseItem> ItemClass;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	int32 MonsterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterMaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterMoveSpeed = 43.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterChaseSpeed = 43.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterAttackSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	bool bIsAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	bool bIsHit = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	bool bIsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	bool bIsInitialSpawn = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	bool bCanDropReward = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	FName MonsterType;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	EMonsterState MonsterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effects")
	UParticleSystem* AttackParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effects")
	UParticleSystem* TakeDamageParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Sound")
	USoundBase* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Sound")
	USoundBase* TakeDamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Animation")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Animation")
	UAnimMontage* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Animation")
	UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Item")
	float MonsterExpReward = 25.0f + (MonsterLevel * 20.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Item")
	float MonsterGoldReward = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Item")
	float MonsterHealKitProbability = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Item")
	float MonsterGoldProbability = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|UI")
	UWidgetComponent* MonsterOverHeadWidget;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	void MonsterDestroy();
	void DropReward();

	void MonsterHit();
	void MonsterHitEnd();

	void UpdateMonsterOverHeadWidget();
	void UpdateMonsterOverHeadWidgetEnd();

	virtual void PlayParticle();
	virtual void PlaySound();

	UFUNCTION(BlueprintCallable)
	void SetInitialSpawn();

	UFUNCTION(BlueprintCallable)
	void UpdateChaseSpeed();

	UFUNCTION(BlueprintCallable)
	void UpdatePatrolSpeed();

	UFUNCTION(BlueprintCallable)
	void UpdateState(EMonsterState NewMonsterState);

	UFUNCTION(BlueprintCallable)
	void MonsterAttack();

	UFUNCTION(BlueprintCallable)
	void SetChaseMode(bool Mode);

	void MonsterAttackEnd();

	FTimerHandle DeadAnimTimerHandle;
	FTimerHandle AttackAnimTimerHandle;
	FTimerHandle HitAnimTimerHandle;
	FTimerHandle OverHeadUITimerHandle;
};
