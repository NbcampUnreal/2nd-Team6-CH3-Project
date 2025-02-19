// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MissionListWidget.h"
#include "Components/Button.h"
#include "System/UIHandle.h"

void UMissionListWidget::InitWidget(UUIHandle* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	BindToAnimationStarted(CloseAnim, StartRemoveDelegate);
	BindToAnimationFinished(CloseAnim, EndRemoveDelegate);
	BindToAnimationStarted(OpenAnim, StartAddDelegate);
	BindToAnimationFinished(OpenAnim, EndAddDelegate);

	MissionButton1->OnClicked.AddDynamic(this, &ThisClass::OnClickedMission1);
	MissionButton2->OnClicked.AddDynamic(this, &ThisClass::OnClickedMission2);
	MissionButton3->OnClicked.AddDynamic(this, &ThisClass::OnClickedMission3);
	MissionButton4->OnClicked.AddDynamic(this, &ThisClass::OnClickedInfinity);
	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedClose);
	EnterButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedEnter);
}

void UMissionListWidget::PlayAddAnim()
{
	Super::PlayAddAnim();

	if (bIsPlaying)
	{
		return;
	}

	PlayAnimation(OpenAnim);
}

void UMissionListWidget::PlayRemoveAnim(bool bIsNext, ESceneType SceneType)
{
	Super::PlayRemoveAnim(bIsNext, SceneType);

	if (bIsPlaying)
	{
		return;
	}

	PlayAnimation(CloseAnim);
}

void UMissionListWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();

	UIHandle->RemoveCoverFromViewport(EWidgetType::MissionListWidget);
}

void UMissionListWidget::OnClickedMission1()
{
	TargetScene = ESceneType::Mission1;
}

void UMissionListWidget::OnClickedMission2()
{
	TargetScene = ESceneType::Mission2;
}

void UMissionListWidget::OnClickedMission3()
{
	TargetScene = ESceneType::Mission3;
}

void UMissionListWidget::OnClickedInfinity()
{
	TargetScene = ESceneType::Infinity;
}

void UMissionListWidget::OnClickedClose()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->RequestRemoveCoverFromViewport(EWidgetType::MissionListWidget);
}

void UMissionListWidget::OnClickedEnter()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedMoveToMission(TargetScene);
	TargetScene = ESceneType::Title;
}
