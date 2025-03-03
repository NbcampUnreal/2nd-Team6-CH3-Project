// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AIInteractionWidget.generated.h"

class UDamageWidget;
class UProgressBar;
class UCanvasPanel;

UCLASS()
class EDMUNDPRJ_API UAIInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitWidget();
	void ApplyHitEvent(int32 MaxHp, int32 CurrentHp, int32 Damage);
	void SetIsVisible(bool IsVisible);

private:
	void UpdateHpBar(int32 MaxHp, int32 CurrentHp);
	void UpdateDamageText(int32 Damage);
	void ResetAllElements();

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UDamageWidget> DamageWidget0;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UDamageWidget> DamageWidget1;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UDamageWidget> DamageWidget2;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCanvasPanel> TotalPanel;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> HpAnimation;

	int32 DamageIndex = 0;
	TArray<TObjectPtr<UDamageWidget>> DamageWidgetSet;
};
