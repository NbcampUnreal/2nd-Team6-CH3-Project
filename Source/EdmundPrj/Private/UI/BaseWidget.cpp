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

void UBaseWidget::InitWidget(UUIHandle* uiHandle)
{
	UIHandle = uiHandle;
	StartAddDelegate.BindDynamic(this, &ThisClass::StartAddAnim);
	EndAddDelegate.BindDynamic(this, &ThisClass::EndAddAnim);
	StartRemoveDelegate.BindDynamic(this, &ThisClass::StartRemoveAnim);
	EndRemoveDelegate.BindDynamic(this, &ThisClass::EndRemoveAnim);

	//BindToAnimationStarted(Animation, Delegate);
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
	
}

void UBaseWidget::PlayRemoveAnim(bool bIsNext, ESceneType SceneType)
{
	
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
