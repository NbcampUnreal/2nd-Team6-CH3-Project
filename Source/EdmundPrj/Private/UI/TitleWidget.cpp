// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleWidget.h"
#include "Components/Button.h"
#include "System/UIHandle.h"

void UTitleWidget::InitWidget(UUIHandle* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	StartButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveNext);
	OptionButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedOptionButton);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedQuitGame);
	//UE_LOG(LogTemp, Warning, TEXT("Title StartButton Name is %s"), *StartButton->GetName());
}

void UTitleWidget::OnClickedOptionButton()
{
	OnClickedOpenWidget(EWidgetType::OptionWidget);
}