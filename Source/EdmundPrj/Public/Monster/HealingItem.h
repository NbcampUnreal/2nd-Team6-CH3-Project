// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/BaseItem.h"
#include "HealingItem.generated.h"

/**
 * 
 */
UCLASS()
class EDMUNDPRJ_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()

	AHealingItem();

public:
	void ActivateItem(AActor* Actor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Healing")
	float HealingPercent = 7.0f;

	float GetHealingPercent() const;
};