// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "System/EnumSet.h"
#include "System/Observer/GameStateObserver.h"
#include "BaseWidget.generated.h"

class UUIHandle;

UCLASS()
class EDMUNDPRJ_API UBaseWidget : public UUserWidget, public IGameStateObserver
{
	GENERATED_BODY()

public:
	virtual void ChangedPlayerMaxHp(int32 Hp) override;
	virtual void ChangedPlayerCurrentHp(int32 Hp) override;
	virtual void ChangedPlayerMaxAmmo(int32 Ammo) override;
	virtual void ChangedPlayerAmmo(int32 Ammo) override;
	virtual void ChangedPlayerMoney(int32 Money) override;
	virtual void InitWidget(UUIHandle* uiHandle);
	virtual void Action();
	virtual void Update();
	virtual void PlayAddAnim();
	virtual void PlayRemoveAnim();
	
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

	TObjectPtr<UWidgetAnimation> OpenAnimation;
	TObjectPtr<UWidgetAnimation> CloseAnimation;

	TObjectPtr<UUIHandle> UIHandle;
	//ESceneType MoveTargetScene = ESceneType::Title;
	bool bIsPlaying = false;
};
