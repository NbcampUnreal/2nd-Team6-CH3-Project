// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MissionListWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "System/UIHandle.h"

void UMissionListWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = OpenAnim;
	CloseAnimation = CloseAnim;

	Super::InitWidget(NewUIHandle);

	MissionButton1->OnClicked.AddDynamic(this, &ThisClass::OnClickedMission1);
	MissionButton2->OnClicked.AddDynamic(this, &ThisClass::OnClickedMission2);
	MissionButton3->OnClicked.AddDynamic(this, &ThisClass::OnClickedMission3);
	MissionButton4->OnClicked.AddDynamic(this, &ThisClass::OnClickedInfinity);
	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedClose);
	EnterButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedEnter);

	MissionButton2->SetIsEnabled(false);
	MissionButton3->SetIsEnabled(false);
	MissionButton4->SetIsEnabled(false);
}

void UMissionListWidget::PlayAddAnim()
{
	Super::PlayAddAnim();
	EnterButton->SetIsEnabled(false);
	CheckCleard();
	OnSelectedImage(0);
}

void UMissionListWidget::PlayRemoveAnim()
{
	Super::PlayRemoveAnim();
	OnSelectedImage(0);
	UIHandle->RequestPlayUISound(EUISoundType::Open);
}

void UMissionListWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();

	UIHandle->RemoveCoverFromViewport(EWidgetType::MissionListWidget);
}

void UMissionListWidget::OnClickedMission1()
{
	EnterButton->SetIsEnabled(true);
	TargetScene = ESceneType::Mission1;
	OnSelectedImage(1);
}

void UMissionListWidget::OnClickedMission2()
{
	EnterButton->SetIsEnabled(true);
	TargetScene = ESceneType::Mission2;
	OnSelectedImage(2);
}

void UMissionListWidget::OnClickedMission3()
{
	EnterButton->SetIsEnabled(true);
	TargetScene = ESceneType::Mission3;
	OnSelectedImage(3);
}

void UMissionListWidget::OnClickedInfinity()
{
	EnterButton->SetIsEnabled(true);
	TargetScene = ESceneType::Infinity;
	OnSelectedImage(4);
}

void UMissionListWidget::OnClickedClose()
{
	OnClickedCloseWidget(EWidgetType::MissionListWidget);
}

void UMissionListWidget::OnClickedEnter()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->RequestPlayUISound(EUISoundType::Click);
	UIHandle->ClickedMoveToMission(TargetScene);
}

void UMissionListWidget::OnSelectedImage(int32 Index)
{
	SelectedImage1->SetVisibility(ESlateVisibility::Collapsed);
	SelectedImage2->SetVisibility(ESlateVisibility::Collapsed);
	SelectedImage3->SetVisibility(ESlateVisibility::Collapsed);
	SelectedImage4->SetVisibility(ESlateVisibility::Collapsed);

	switch (Index)
	{
	case 0:
		
		break;

	case 1:
		SelectedImage1->SetVisibility(ESlateVisibility::Visible);
		break;

	case 2:
		SelectedImage2->SetVisibility(ESlateVisibility::Visible);
		break;

	case 3:
		SelectedImage3->SetVisibility(ESlateVisibility::Visible);
		break;

	case 4:
		SelectedImage4->SetVisibility(ESlateVisibility::Visible);
		break;

	default:
		checkNoEntry();
		break;
	}
}

void UMissionListWidget::CheckCleard()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));

	if (UIHandle->CheckClearedMission(0))
	{
		MissionButton2->SetIsEnabled(true);
	}

	if (UIHandle->CheckClearedMission(1))
	{
		MissionButton3->SetIsEnabled(true);
	}

	if (UIHandle->CheckClearedMission(2))
	{
		MissionButton4->SetIsEnabled(true);
	}
}
