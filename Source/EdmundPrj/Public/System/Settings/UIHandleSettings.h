// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UIHandleSettings.generated.h"

class UBaseWidget;

UCLASS(Config = UISetting, DefaultConfig, meta = (DisplayName = "UIHandleSettings"))
class EDMUNDPRJ_API UUIHandleSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Config, Category = "WidgetClass")
	TSubclassOf<UBaseWidget> TitleWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "WidgetClass")
	TSubclassOf<UBaseWidget> MainWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "WidgetClass")
	TSubclassOf<UBaseWidget> InGameWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "WidgetClass")
	TSubclassOf<UBaseWidget> EndingWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "WidgetClass")
	TSubclassOf<UBaseWidget> FadeWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "WidgetClass")
	TSubclassOf<UBaseWidget> ShopWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "WidgetClass")
	TSubclassOf<UBaseWidget> OptionWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "WidgetClass")
	TSubclassOf<UBaseWidget> ResultWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "WidgetClass")
	TSubclassOf<UBaseWidget> TextWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "WidgetClass")
	TSubclassOf<UBaseWidget> CharacterListWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "WidgetClass")
	TSubclassOf<UBaseWidget> SkillListWidgetClass = nullptr;
};
