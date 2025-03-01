// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Plant.h"
#include "HealPlant.generated.h"

class ABaseCharacter;

UCLASS()
class EDMUNDPRJ_API AHealPlant : public APlant
{
	GENERATED_BODY()
	
public:
	AHealPlant();
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
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<USphereComponent> CheckCharacterCollision = nullptr;
	FTimerHandle HealCycleHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float HealAmount = 0;
};
