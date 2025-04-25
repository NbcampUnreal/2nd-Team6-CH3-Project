// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TimerSkill.h"
#include "Meteor.generated.h"

class UParticleSystem;
class UNiagaraComponent;

UCLASS()
class EDMUNDPRJ_API AMeteor : public ATimerSkill
{
	GENERATED_BODY()
	AMeteor();
	virtual void HitToMonster(TObjectPtr<ABaseMonster> Monster) override;
	virtual void HitToGround() override;
	void Deactivate();
	virtual void SpawnTimerSkill() override;
	virtual void Tick(float deltaTime);
	virtual void UpgradeSkill() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float MetoerHitPushStrength = 0;



private:
	UPROPERTY()
	TSet<TObjectPtr<ABaseMonster>> HitMonsterSet = {};

	FTimerHandle ElplosionEffectDeactivateHandle;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UNiagaraComponent> FallingEffectNiagara = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UNiagaraComponent> GroundHitNiagara = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setting")
	TObjectPtr<USphereComponent> MonsterLaunchCollision = nullptr;
};
