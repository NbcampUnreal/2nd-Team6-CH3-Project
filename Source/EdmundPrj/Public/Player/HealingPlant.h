// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TimerSkill.h"
#include "HealingPlant.generated.h"

class ABaseCharacter;

UCLASS()
class EDMUNDPRJ_API AHealingPlant : public ATimerSkill
{
	GENERATED_BODY()
public:
	AHealingPlant();
	UFUNCTION()
	void BeginOverlapCharacter(
		UPrimitiveComponent* overlappedComp,
		AActor* otherActor,
		UPrimitiveComponent* otherComp,
		int32 otherBodyIndex,
		bool bFromSweep, const
		FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlapCharacter(UPrimitiveComponent*
		overlappedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComp,
		int32 otherBodyIndex);
	void HealingCharacter();
	void SpawnHealingPlant();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<USphereComponent> CheckCharacterCollision = nullptr;
	FTimerHandle HealCycleHandle;
	FTimerHandle SpawnShakeHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float HealAmount = 0;
};
