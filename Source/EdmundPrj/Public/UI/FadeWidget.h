// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "FadeWidget.generated.h"

class UUIHandle;

UCLASS()
class EDMUNDPRJ_API UFadeWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUIHandle* NewUIHandle) override;
	virtual void PlayRemoveAnim(bool bIsNext = false, ESceneType SceneType = ESceneType::Title) override; //FadeOut
	virtual void PlayAddAnim() override; //FadeIn
	virtual void EndRemoveAnim() override;
	virtual void EndAddAnim() override;

private:
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeInAnim;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOutAnim;

	bool bIsFadeNext = false;
	ESceneType FadeSceneType = ESceneType::Title;
};
