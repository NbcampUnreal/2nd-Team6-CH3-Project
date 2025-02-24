// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubWidget/SkillWidget.h"
#include "UI/SkillListWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USkillWidget::InitSkill(USkillListWidget* ListWidget, int32 SkillNum)
{
	SkillListWidget = ListWidget;
	SkillNumber = SkillNum;
	SelectSkillButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedButton);
}

void USkillWidget::UpdateSkill(UTexture2D* SkillImage, const FName& SkillName)
{
	SkillNameText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *SkillName.ToString())));
	SelectSkillButton->WidgetStyle.Normal.SetResourceObject(SkillImage);
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
