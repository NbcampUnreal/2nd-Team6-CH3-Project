// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopInfoWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class EDMUNDPRJ_API UShopInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitInfo(UTexture2D* Texture, const FName& InfoName);
	void UpdateInfoValue(const int32 Value);
	void UpdateInfoValue(const float Value);

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UImage> InfoImage;
};
