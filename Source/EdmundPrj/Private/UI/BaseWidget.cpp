// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseWidget.h"
#include "System/UIHandle.h"

void UBaseWidget::ChangedPlayerMaxHp(int32 Hp)
{

}

void UBaseWidget::ChangedPlayerCurrentHp(int32 Hp)
{

}

void UBaseWidget::ChangedPlayerMaxAmmo(int32 Ammo)
{

}

void UBaseWidget::ChangedPlayerAmmo(int32 Ammo)
{

}

void UBaseWidget::ChangedPlayerMoney(int32 Money)
{

}

void UBaseWidget::ChangedSkillList()
{
}

void UBaseWidget::InitWidget(UUIHandle* uiHandle)
{
	UIHandle = uiHandle;
	StartAddDelegate.BindDynamic(this, &ThisClass::StartAddAnim);
	EndAddDelegate.BindDynamic(this, &ThisClass::EndAddAnim);
	StartRemoveDelegate.BindDynamic(this, &ThisClass::StartRemoveAnim);
	EndRemoveDelegate.BindDynamic(this, &ThisClass::EndRemoveAnim);

	BindToAnimationStarted(OpenAnimation, StartAddDelegate);
	BindToAnimationFinished(OpenAnimation, EndAddDelegate);
	BindToAnimationStarted(CloseAnimation, StartRemoveDelegate);
	BindToAnimationFinished(CloseAnimation, EndRemoveDelegate);
}

void UBaseWidget::Action()
{

}

void UBaseWidget::StartAddAnim()
{
	bIsPlaying = true;
}

void UBaseWidget::EndAddAnim()
{
	bIsPlaying = false;
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
}

void UBaseWidget::Update()
{

}

void UBaseWidget::PlayAddAnim()
{
	if (bIsPlaying)
	{
		return;
	}
	PlayAnimation(OpenAnimation);
}

void UBaseWidget::PlayRemoveAnim(bool bIsNext, ESceneType SceneType)
{
	if (bIsPlaying)
	{
		return;
	}
	PlayAnimation(CloseAnimation);
}

void UBaseWidget::PlayRemoveAnim()
{
	if (bIsPlaying)
	{
		return;
	}
	PlayAnimation(CloseAnimation);
}

void UBaseWidget::StartRemoveAnim()
{
	bIsPlaying = true;
}

void UBaseWidget::EndRemoveAnim()
{
	bIsPlaying = false;
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
}

void UBaseWidget::OnClickedMoveNext()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedMoveToNext();
}

void UBaseWidget::OnClickedMoveMain()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedMoveToMain();
}

void UBaseWidget::OnClickedMoveTitle()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedMoveToTitle();
}

void UBaseWidget::OnClickedQuitGame()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedQuitGame();
}

void UBaseWidget::OnClickedOpenWidget(const EWidgetType WidgetType)
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->AddToViewportByCoverType(WidgetType);
}

void UBaseWidget::OnClickedCloseWidget(const EWidgetType WidgetType)
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->RequestRemoveCoverFromViewport(WidgetType);
}
