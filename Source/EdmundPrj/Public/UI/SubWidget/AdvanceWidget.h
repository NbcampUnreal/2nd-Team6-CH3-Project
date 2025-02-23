// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AdvanceWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UShopWidget;

UCLASS()
class EDMUNDPRJ_API UAdvanceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitWidget(const FName& WidgetName, UShopWidget* Shop);
	void UpdateSkillImage(UTexture2D* Texture);
	void UpdatePrice(int32 Price);
	void UpdateInfo(const FString& Text);
	const FName& GetElementName() const;

private:
	UFUNCTION()
	void OnClickedBuy();

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> BuyButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UImage> SkillImage;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> BuyButtonText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> InfoText;

	TObjectPtr<UShopWidget> ShopWidget;
	FName ItemName = "";

};
