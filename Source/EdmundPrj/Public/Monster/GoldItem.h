// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/BaseItem.h"
#include "GoldItem.generated.h"

/**
 * 
 */
UCLASS()
class EDMUNDPRJ_API AGoldItem : public ABaseItem
{
	GENERATED_BODY()

	AGoldItem();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Gold")
	float GoldAmount = 10.0f;

	float GetGoldAmount() const;

	void SetGoldAmount(float NewGold);

	void ActivateItem(AActor* Actor) override;




};
