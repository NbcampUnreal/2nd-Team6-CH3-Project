// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillListWidget.h"
#include "System/UIHandle.h"
#include "UI/SubWidget/SkillWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "System/DataStructure/PlayerSkillRow.h"

void USkillListWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = OpenAnim;
	CloseAnimation = CloseAnim;

	Super::InitWidget(NewUIHandle);

	CreateSkillWidget();
	InvisibleAllBorder();

	SelectButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedSelectButton);
}

void USkillListWidget::PlayAddAnim()
{
	Super::PlayAddAnim();
	InvisibleAllBorder();
	SelectButton->SetIsEnabled(false);
}

void USkillListWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();
	UIHandle->RemoveCoverFromViewport(EWidgetType::SkillListWidget);
}

void USkillListWidget::ChangedSkillList()
{
	Super::ChangedSkillList();

	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	TArray<FPlayerSkillRow*> RandomSkillSet = UIHandle->GetCurrentRandomSkill();

	for (int32 i = 0; i < 3; i++)
	{
		FPlayerSkillRow* SkillRow = RandomSkillSet[i];

		SkillElements[i]->UpdateSkill(SkillRow->SkillImage, SkillRow->SkillName);
	}
}

void USkillListWidget::OnClickedSkillButton(USkillWidget* SkillWidget)
{
	SelectedSkill = SkillWidget;
	InvisibleAllBorder();
	SelectButton->SetIsEnabled(true);
}

void USkillListWidget::OnClickedSelectButton()
{
	if (!IsValid(SelectedSkill))
	{
		return;
	}

	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedSelectSkill(SelectedSkill->GetSkillNumber());
	SelectedSkill->InvisibleBorder();
	SelectedSkill = nullptr;
	OnClickedCloseWidget(EWidgetType::SkillListWidget);
}

void USkillListWidget::CreateSkillWidget()
{
	SkillWidgetClass = BaseSkill->GetClass();
	
	for (int32 i = 0; i < 3; i++)
	{
		TObjectPtr<USkillWidget> NewElement = CreateWidget<USkillWidget>(this, SkillWidgetClass);
		SkillList->AddChildToUniformGrid(NewElement, 0, i);
		SkillElements.Add(NewElement);

		NewElement->InitSkill(this, i);
	}
}

void USkillListWidget::InvisibleAllBorder()
{
	for (USkillWidget* TargetSkillWidget : SkillElements)
	{
		TargetSkillWidget->InvisibleBorder();
	}
}
