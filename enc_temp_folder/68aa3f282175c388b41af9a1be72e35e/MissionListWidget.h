// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "MissionListWidget.generated.h"

class UButton;
class UUIHandle;

UCLASS()
class EDMUNDPRJ_API UMissionListWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUIHandle* NewUIHandle) override;
	virtual void PlayAddAnim() override;
	virtual void PlayRemoveAnim(bool bIsNext = false, ESceneType SceneType = ESceneType::Title) override;
	virtual void EndRemoveAnim() override;

private:
	UFUNCTION()
	void OnClickedMission1();

	UFUNCTION()
	void OnClickedMission2();

	UFUNCTION()
	void OnClickedMission3();

	UFUNCTION()
	void OnClickedInfinity();

	UFUNCTION()
	void OnClickedClose();

	UFUNCTION()
	void OnClickedEnter();

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MissionButton1;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MissionButton2;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MissionButton3;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MissionButton4;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> EnterButton;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenAnim;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> CloseAnim;

	ESceneType TargetScene = ESceneType::Title;
};
