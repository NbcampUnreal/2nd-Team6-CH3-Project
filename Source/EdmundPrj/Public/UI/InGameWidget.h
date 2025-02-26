// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "InGameWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class EDMUNDPRJ_API UInGameWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void ChangedNotifyText(const FString& NotifyText) override;
	virtual void ChangedMissionText(const FString& MissionText) override;
	virtual void ChangedPlayerHp(const int32 MaxHp, const int32 CurrentHp) override;
	virtual void ChangedPlayerOther(const int32 MaxValue, const int32 CurrentValue) override;
	virtual void ChangedPlayerAmmo(const int32 MaxAmmo, const int32 CurrentAmmo) override;


private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> MissionInfoText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> NotifyTextBlock;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> HpText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> OtherText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UProgressBar> OtherBar;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> NotifyAnimation;
};
