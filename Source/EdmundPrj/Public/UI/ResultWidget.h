// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "ResultWidget.generated.h"

class UUIHandle;
class UButton;
class UTextBlock;

UCLASS()
class EDMUNDPRJ_API UResultWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public: // add notify
	virtual void InitWidget(UUIHandle* NewUIHandle) override;

private: 
	UFUNCTION()
	void OnClickedRetry();

	void UpdateResult(bool bIsClear);

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveNextButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveMainButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveNextButtonText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenAnim;
};
