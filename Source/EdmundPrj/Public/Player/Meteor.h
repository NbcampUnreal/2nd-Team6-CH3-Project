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
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float MetoerHitPushStrength = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<USphereComponent> MonsterLaunchCollision = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UNiagaraComponent> GroundHitNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UNiagaraComponent> FallingEffectNiagara;
	FTimerHandle ElplosionEffectDeactivateHandle;
};
