// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubWidget/SkillWidget.h"
#include "UI/SkillListWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"

void USkillWidget::InitSkill(USkillListWidget* ListWidget, int32 SkillNum)
{
	SkillListWidget = ListWidget;
	SkillNumber = SkillNum;
	SelectSkillButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedButton);
	SelectSkillButton->OnHovered.AddDynamic(this, &ThisClass::OnHoveredButton);
	SelectSkillButton->OnUnhovered.AddDynamic(this, &ThisClass::OnUnhoveredButton);
}

void USkillWidget::UpdateSkill(UTexture2D* SkillImage, const FName& SkillName, const FString& SkillInfo)
{
	SkillNameText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *SkillName.ToString())));
	InfoText->SetText(FText::FromString(SkillInfo));
	SelectSkillButton->WidgetStyle.Normal.SetResourceObject(SkillImage);
	SelectSkillButton->WidgetStyle.Hovered.SetResourceObject(SkillImage);
	SelectSkillButton->WidgetStyle.Pressed.SetResourceObject(SkillImage);
	SkillInfoBox->SetVisibility(ESlateVisibility::Collapsed);
	InvisibleBorder();
}

void USkillWidget::InvisibleBorder()
{
	Border->SetVisibility(ESlateVisibility::Collapsed);
}

const int32 USkillWidget::GetSkillNumber() const
{
	return SkillNumber;
}

void USkillWidget::OnClickedButton()
{
	SkillListWidget->OnClickedSkillButton(this);
	Border->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::OnHoveredButton()
{
	SkillInfoBox->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::OnUnhoveredButton()
{
	SkillInfoBox->SetVisibility(ESlateVisibility::Collapsed);
}
