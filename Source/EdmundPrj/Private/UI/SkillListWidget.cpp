// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillListWidget.h"
#include "System/UIHandle.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USkillListWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = OpenAnim;
	CloseAnimation = CloseAnim;

	Super::InitWidget(NewUIHandle);

	SelectButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedSelectButton);
	SelectButton1->OnClicked.AddDynamic(this, &ThisClass::OnClickedSkillButton1);
	SelectButton2->OnClicked.AddDynamic(this, &ThisClass::OnClickedSkillButton2);
	SelectButton3->OnClicked.AddDynamic(this, &ThisClass::OnClickedSkillButton3);

	OnSelectedImage(0);
}

void USkillListWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();
	UIHandle->RemoveCoverFromViewport(EWidgetType::SkillListWidget);
}

void USkillListWidget::OnClickedSelectButton()
{
	OnSelectedImage(0);
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedSelectSkill();
	OnClickedCloseWidget(EWidgetType::SkillListWidget);
}

void USkillListWidget::OnClickedSkillButton1()
{
	OnSelectedImage(1);
}

void USkillListWidget::OnClickedSkillButton2()
{
	OnSelectedImage(2);
}

void USkillListWidget::OnClickedSkillButton3()
{
	OnSelectedImage(3);
}

void USkillListWidget::OnSelectedImage(int32 Index)
{
	Border1->SetVisibility(ESlateVisibility::Collapsed);
	Border2->SetVisibility(ESlateVisibility::Collapsed);
	Border3->SetVisibility(ESlateVisibility::Collapsed);

	switch (Index)
	{
	case 0:

		break;

	case 1:
		Border1->SetVisibility(ESlateVisibility::Visible);
		break;

	case 2:
		Border2->SetVisibility(ESlateVisibility::Visible);
		break;

	case 3:
		Border3->SetVisibility(ESlateVisibility::Visible);
		break;

	default:
		checkNoEntry();
		break;
	}
}