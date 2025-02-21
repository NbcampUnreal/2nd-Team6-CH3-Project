// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FadeWidget.h"
#include "System/UIHandle.h"

void UFadeWidget::InitWidget(UUIHandle* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	BindToAnimationStarted(FadeOutAnim, StartRemoveDelegate);
	BindToAnimationFinished(FadeOutAnim, EndRemoveDelegate);
	BindToAnimationStarted(FadeInAnim, StartAddDelegate);
	BindToAnimationFinished(FadeInAnim, EndAddDelegate);
}

void UFadeWidget::PlayFadeOutAnim(bool bIsNext, ESceneType SceneType)
{
	bIsFadeNext = bIsNext;
	FadeSceneType = SceneType;
	PlayAnimation(FadeOutAnim);
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
