// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleWidget.h"
#include "Components/Button.h"
#include "System/UIHandle.h"

void UTitleWidget::InitWidget(UUIHandle* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	StartButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedStartButton);
	OptionButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedOptionButton);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedQuitButton);
}

void UTitleWidget::OnClickedStartButton()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));

	UIHandle->ClickedMoveToNext();
}

void UTitleWidget::OnClickedOptionButton()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));

	//UIHandle->OpenOption();
	UIHandle->AddToViewportByCoverType(EWidgetType::OptionWidget);
}

void UTitleWidget::OnClickedQuitButton()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));

	UIHandle->ClickedQuitGame();
}
