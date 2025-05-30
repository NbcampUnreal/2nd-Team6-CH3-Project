// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TimerSkill.h"
#include "SolarDisk.generated.h"

class UNiagaraComponent;

UCLASS()
class EDMUNDPRJ_API ASolarDisk : public ATimerSkill
{
	GENERATED_BODY()
public:
	ASolarDisk();
	void HitToMonsterInCollision();
	virtual void SpawnTimerSkill() override;
	virtual void Deactivate() override;
	virtual void UpgradeSkill() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	FTimerHandle AttackCycleHandle;
	FTimerHandle SpawnAnimHandle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<UStaticMeshComponent> BaseMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<UStaticMeshComponent> TowerMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<UStaticMeshComponent> SphereMesh = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float DamageCycleTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UNiagaraComponent> HitNiagara;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracking")
	float RotationSpeed = 5.0f;
};
