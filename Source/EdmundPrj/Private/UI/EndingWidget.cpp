// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EndingWidget.h"
#include "Components/Button.h"

void UEndingWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = CreditAnimation;

	Super::InitWidget(NewUIHandle);

	MoveMainButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveMain);
	QuitGameButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedQuitGame);
}
