// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "TitleWidget.generated.h"

class UButton;

UCLASS()
class EDMUNDPRJ_API UTitleWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUIHandle* NewUIHandle) override;

private:
	UFUNCTION()
	void OnClickedStartButton();

	UFUNCTION()
	void OnClickedOptionButton();

	UFUNCTION()
	void OnClickedQuitButton();

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> OptionButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
};
