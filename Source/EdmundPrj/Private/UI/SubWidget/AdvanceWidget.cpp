// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubWidget/AdvanceWidget.h"
#include "UI/ShopWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UAdvanceWidget::InitWidget(const FName& WidgetName, UShopWidget* Shop)
{
	ItemName = WidgetName;
	ShopWidget = Shop;
	BuyButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedBuy);
}

void UAdvanceWidget::UpdateSkillImage(UTexture2D* Texture)
{
	SkillImage->SetBrushFromTexture(Texture);
}

void UAdvanceWidget::UpdatePrice(int32 Price)
{
	BuyButtonText->SetText(FText::FromString(FString::FromInt(Price)));
}

void UAdvanceWidget::UpdateInfo(const FString& Text)
{
	InfoText->SetText(FText::FromString(Text));
}

const FName& UAdvanceWidget::GetElementName() const
{
	return ItemName;
}

void UAdvanceWidget::ApplyMaxLevel()
{
	BuyButtonText->SetText(FText::FromString(FString::Printf(TEXT("Max"))));
	BuyButton->SetIsEnabled(false);
}

void UAdvanceWidget::OnClickedBuy()
{
	checkf(IsValid(ShopWidget), TEXT("ShopWidget is invalid"));
	ShopWidget->OnClickedBuyButton(this);
}
