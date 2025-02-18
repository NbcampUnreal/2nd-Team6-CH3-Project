// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionWidget.h"
#include "System/UIHandle.h"
#include "Components/Button.h"

void UOptionWidget::InitWidget(UUIHandle* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	MoveTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveToTitle);
	MoveMainButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveToMain);
	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCloseOption);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedQuitGame);
}

void UOptionWidget::OnClickedMoveToMain()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is not valid"));
	UIHandle->ClickedMoveToMain();
}

void UOptionWidget::OnClickedMoveToTitle()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is not valid"));
	UIHandle->ClickedMoveToTitle();
}

void UOptionWidget::OnClickedQuitGame()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is not valid"));
	UIHandle->ClickedQuitGame();
}

void UOptionWidget::OnClickedCloseOption()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is not valid"));
	UIHandle->CloseOption();
}
