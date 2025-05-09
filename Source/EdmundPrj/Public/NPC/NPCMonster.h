// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/BaseMonster.h"
#include "NPCMonster.generated.h"

class ANPCAttack;
class UCapsuleComponent;
class AMissionHandle;

UCLASS()
class EDMUNDPRJ_API ANPCMonster : public ABaseMonster
{
	GENERATED_BODY()

public:
	void SetMonsterStatsByLevel() override;

	ANPCMonster();

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Components")
	UCapsuleComponent* CollisionComp;*/

	void BeginPlay() override;

	void MonsterAttackCheck() override;

	UFUNCTION(BlueprintCallable)
	void SetBattleMode(bool NewState);

	UFUNCTION(BlueprintCallable)
	void SetBondageMode(bool NewState);

	UFUNCTION(BlueprintCallable)
	void SetMoveMode(bool NewState);

	UFUNCTION(BlueprintCallable)
	void InitSpawnParticlePlay();

	void PlayBondageMontage();

	void PlaySound();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC")
	ENpcType NpcType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Animation")
	UAnimMontage* BondageAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Particle")
	UParticleSystem* SpawnParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Particle")
	UParticleSystem* InitSpawnParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Attack")
	TSubclassOf<ANPCAttack> AttackClass;

	bool bIsAttacked = false;

	FTimerHandle BondageTimerHandle;

	//void SetAttackedfalse();

	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp, //동전
		AActor* OtherActor, //플레이어
		UPrimitiveComponent* OtherComp, //플레이어의 Collision(Capsule Component)
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void NPCMonsterAttack();

	virtual void MonsterDead() override;

	void SetMissionHandle(AMissionHandle* NewMissionHandle);

	TObjectPtr<AMissionHandle> MissionHandle;
	//void PlayParticle() override;
	//void PlaySound() override;
};
