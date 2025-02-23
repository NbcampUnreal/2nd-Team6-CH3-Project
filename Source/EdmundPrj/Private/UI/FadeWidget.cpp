// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FadeWidget.h"
#include "System/UIHandle.h"

void UFadeWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = FadeInAnim;
	CloseAnimation = FadeOutAnim;
	Super::InitWidget(NewUIHandle);
}

void UFadeWidget::PlayRemoveAnim(bool bIsNext, ESceneType SceneType)
{
	bIsFadeNext = bIsNext;
	FadeSceneType = SceneType;
	Super::PlayRemoveAnim(bIsNext, SceneType);
}

void UFadeWidget::PlayAddAnim()
{
	PlayAnimation(FadeInAnim);
}

void UFadeWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();

	UIHandle->RequestMoveSceneByFade(bIsFadeNext, FadeSceneType);
}

void UFadeWidget::EndAddAnim()
{
	Super::EndAddAnim();

	UIHandle->RemoveCoverFromViewport(EWidgetType::FadeWidget);
}
