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
	checkf(IsValid(UIHandle), TEXT("UIHandle is not valid"));

	UIHandle->ClickedMoveToNext();
}

void UTitleWidget::OnClickedOptionButton()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is not valid"));

	UIHandle->OpenOption();
}

void UTitleWidget::OnClickedQuitButton()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is not valid"));

	UIHandle->ClickedQuitGame();
}
