// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubWidget/ShopInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UShopInfoWidget::InitInfo(UTexture2D* Texture, const FName& InfoName)
{
	InfoImage->SetBrushFromTexture(Texture);
	NameText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *InfoName.ToString())));
}

void UShopInfoWidget::UpdateInfoValue(const int32 Value)
{
	ValueText->SetText(FText::FromString(FString::Printf(TEXT("+ %d"), Value)));
}

void UShopInfoWidget::UpdateInfoValue(const float Value)
{
	ValueText->SetText(FText::FromString(FString::Printf(TEXT("+ %.2f"), Value)));
}
