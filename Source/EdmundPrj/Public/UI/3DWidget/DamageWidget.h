// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageWidget.generated.h"

class UTextBlock;

UCLASS()
class EDMUNDPRJ_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ApplyDamage(int32 DamageValue);
	void ResetDamage();

private:
	void PlayHitAnim();
	void SetDamageText(int32 DamageValue);

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageText;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> HitAnimation;
};
