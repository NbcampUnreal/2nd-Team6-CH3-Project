// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "InGameWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UCanvasPanel;

UCLASS()
class EDMUNDPRJ_API UInGameWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void Update() override;
	virtual void ChangedNotifyText(const FString& NotifyText) override;
	virtual void ChangedMissionText(const FString& MissionText) override;
	virtual void ChangedPlayerHp(const int32 MaxHp, const int32 CurrentHp) override;
	virtual void ChangedPlayerOther(const int32 MaxValue, const int32 CurrentValue) override;
	virtual void ChangedPlayerAmmo(const int32 MaxAmmo, const int32 CurrentAmmo) override;
	virtual void ChangedPlayerExp(const int32 MaxExp, const int32 CurrentExp) override;
	virtual void ChangedPlayerLevel(const int32 LevelValue) override;
	virtual void ChangedCharacterType(const ECharacterType CharacterType) override;
	virtual void ChangedStageToProgress(const FString& ProgressText, const bool bIsOn) override;
	virtual void ChangedBossHp(const int32 MaxHp, const int32 CurrentHp) override;
	virtual void ChangedMissionInfoOnOff() override;

private:
	void SetAmmoTextVisibility(bool bIsVisible);

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
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> BossHpBarText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UProgressBar> OtherBar;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UProgressBar> ExpBar;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UProgressBar> BossHpBar;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCanvasPanel> BossHpBox;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> NotifyAnimation;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> BossHpAnimation;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> MissionInfoAnimation;

	bool bIsMaxLevel = false;
	bool bOnMissionInfo = true;
};
