// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "MainWidget.generated.h"

class UUIHandle;
class UButton;

UCLASS()
class EDMUNDPRJ_API UMainWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUIHandle* NewUIHandle) override;

private:
	UFUNCTION()
	void OnClickedShop();

	UFUNCTION()
	void OnClickedCharacterList();

	UFUNCTION()
	void OnClickedMissionList();

	UFUNCTION()
	void OnClickedOption();

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> EnterShopButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> EnterSelectMissionButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> OpenSelectCharacterButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> OptionButton;
};
