// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FadeWidget.h"
#include "System/UIHandle.h"

void UFadeWidget::InitWidget(UUIHandle* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	BindToAnimationFinished(FadeOutAnim, EndRemoveDelegate);
	BindToAnimationFinished(FadeInAnim, EndAddDelegate);
}

void UFadeWidget::PlayRemoveAnim(bool bIsNext, ESceneType SceneType)
{
	Super::PlayRemoveAnim();

	bIsFadeNext = bIsNext;
	FadeSceneType = SceneType;
	PlayAnimation(FadeOutAnim);
}

void UFadeWidget::PlayAddAnim()
{
	Super::PlayAddAnim();

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
