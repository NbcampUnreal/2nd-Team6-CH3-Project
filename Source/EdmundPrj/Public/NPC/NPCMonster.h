// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/BaseMonster.h"
#include "NPCMonster.generated.h"

class ANPCAttack;
class UCapsuleComponent;

UCLASS()
class EDMUNDPRJ_API ANPCMonster : public ABaseMonster
{
	GENERATED_BODY()

public:
	ANPCMonster();

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Components")
	UCapsuleComponent* CollisionComp;*/

	void BeginPlay() override;

	void MonsterAttackCheck() override;

	UFUNCTION(BlueprintCallable)
	void SetBattleMode(bool NewState);

	UFUNCTION(BlueprintCallable)
	void SetBondageMode(bool NewState);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Mode")
	bool bIsFightMode = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Mode")
	bool bIsBondageMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Attack")
	TSubclassOf<ANPCAttack> AttackClass;

	bool bIsAttacked = false;

	//void SetAttackedfalse();

	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp, //동전
		AActor* OtherActor, //플레이어
		UPrimitiveComponent* OtherComp, //플레이어의 Collision(Capsule Component)
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	//void PlayParticle() override;
	//void PlaySound() override;
};
