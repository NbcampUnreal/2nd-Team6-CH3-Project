// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "System/EnumSet.h"
#include "System/Observer/GameStateObserver.h"
#include "BaseWidget.generated.h"

class UUIHandle;
struct FPlayerSkillRow;

UCLASS()
class EDMUNDPRJ_API UBaseWidget : public UUserWidget, public IGameStateObserver
{
	GENERATED_BODY()

public:
	virtual void ChangedPlayerHp(const int32 MaxHp, const int32 CurrentHp) override {};
	virtual void ChangedPlayerAmmo(const int32 MaxAmmo, const int32 CurrentAmmo) override {};
	virtual void ChangedPlayerOther(const int32 MaxValue, const int32 CurrentValue) override {};
	virtual void ChangedPlayerExp(const int32 MaxExp, const int32 CurrentExp) override {};
	virtual void ChangedSkillList() override {};
	virtual void ChangedCharacterType(const ECharacterType CharacterType) override {};
	virtual void ChangedNotifyText(const FString& NotifyText) override {};
	virtual void ChangedMissionText(const FString& MissionText) override {};
	virtual void ChangedMissionStateToEnd(const int32 StateMoney, const int32 InstanceMoney, const int32 MissionMoney) override {};
	virtual void ChangedPlayerLevel(const int32 LevelValue) override {};
	virtual void ChangedStageToProgress(const FString& ProgressText, const bool bIsOn) override {};
	virtual void ChangedBossHp(const int32 MaxHp, const int32 CurrentHp) override {};
	virtual void ChangedMissionInfoOnOff() override {};
	virtual void ChangedStoryText(const FString& TargetText) override {};
	virtual void ChangedIsGameClear(const bool bIsClear) override {};

	virtual void InitWidget(UUIHandle* uiHandle);
	virtual void Action();
	virtual void Update();
	virtual void PlayAddAnim();
	virtual void PlayRemoveAnim();
	virtual void PlayRemoveAnim(bool bIsNext, ESceneType SceneType = ESceneType::Title);
	
protected:
	UFUNCTION()
	virtual void StartAddAnim();

	UFUNCTION()
	virtual void EndAddAnim();

	UFUNCTION()
	virtual void StartRemoveAnim();

	UFUNCTION()
	virtual void EndRemoveAnim();

	UFUNCTION()
	virtual void OnClickedMoveNext();

	UFUNCTION()
	virtual void OnClickedMoveMain();

	UFUNCTION()
	virtual void OnClickedMoveTitle();

	UFUNCTION()
	virtual void OnClickedQuitGame();

	virtual void OnClickedOpenWidget(const EWidgetType WidgetType);
	virtual void OnClickedCloseWidget(const EWidgetType WidgetType);

protected:
	FWidgetAnimationDynamicEvent StartAddDelegate;
	FWidgetAnimationDynamicEvent EndAddDelegate;
	FWidgetAnimationDynamicEvent StartRemoveDelegate;
	FWidgetAnimationDynamicEvent EndRemoveDelegate;

	UPROPERTY()
	TObjectPtr<UWidgetAnimation> OpenAnimation = nullptr;

	UPROPERTY()
	TObjectPtr<UWidgetAnimation> CloseAnimation = nullptr;

	UPROPERTY()
	TObjectPtr<UUIHandle> UIHandle = nullptr;
	
	bool bIsPlaying = false;
};
