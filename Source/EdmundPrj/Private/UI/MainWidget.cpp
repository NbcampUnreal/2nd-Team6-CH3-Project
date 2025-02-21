// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainWidget.h"
#include "System/UIHandle.h"
#include "Components/Button.h"

void UMainWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = OpenAnim;
	CloseAnimation = CloseAnim;

	Super::InitWidget(NewUIHandle);

	EnterShopButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedShop);
	EnterSelectMissionButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMissionList);
	OpenSelectCharacterButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCharacterList);
	OptionButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedOption);
}

void UMainWidget::OnClickedShop()
{
	OnClickedOpenWidget(EWidgetType::ShopWidget);
}

void UMainWidget::OnClickedCharacterList()
{
	OnClickedOpenWidget(EWidgetType::CharacterListWidget);
}

void UMainWidget::OnClickedMissionList()
{
	OnClickedOpenWidget(EWidgetType::MissionListWidget);
}

void UMainWidget::OnClickedOption()
{
	OnClickedOpenWidget(EWidgetType::OptionWidget);
}
