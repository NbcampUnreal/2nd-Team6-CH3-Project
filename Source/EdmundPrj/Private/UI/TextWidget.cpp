// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TextWidget.h"
#include "System/UIHandle.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTextWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = OpenAnim;
	CloseAnimation = CloseAnim;

	Super::InitWidget(NewUIHandle);

	SkipButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedSkipButton);
}

void UTextWidget::ChangedStoryText(const FString& TargetText)
{
	Super::ChangedStoryText(TargetText);
	StoryText->SetText(FText::FromString(TargetText));
}

void UTextWidget::OnClickedNextText()
{

}

void UTextWidget::OnClickedSkipButton()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedSkipStory();
}

void UTextWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();

	UIHandle->RemoveCoverFromViewport(EWidgetType::OptionWidget);
}