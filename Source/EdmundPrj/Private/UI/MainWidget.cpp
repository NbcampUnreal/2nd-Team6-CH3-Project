// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainWidget.h"
#include "System/UIHandle.h"
#include "Components/Button.h"

void UMainWidget::InitWidget(UUIHandle* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	EnterShopButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedShop);
	EnterSelectMissionButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMissionList);
	OpenSelectCharacterButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCharacterList);
	OptionButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedOption);
}

void UMainWidget::OnClickedShop()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->AddToViewportByCoverType(EWidgetType::ShopWidget);
}

void UMainWidget::OnClickedCharacterList()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->AddToViewportByCoverType(EWidgetType::CharacterListWidget);
}

void UMainWidget::OnClickedMissionList()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->AddToViewportByCoverType(EWidgetType::MissionListWidget);
}

void UMainWidget::OnClickedOption()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->AddToViewportByCoverType(EWidgetType::OptionWidget);
	//UIHandle->OpenOption();
}
