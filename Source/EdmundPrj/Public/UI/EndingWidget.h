// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "EndingWidget.generated.h"

class UButton;
class UTextBlock;
class UUIHandle;

UCLASS()
class EDMUNDPRJ_API UEndingWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUIHandle* NewUIHandle) override;
	
private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveMainButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> QuitGameButton;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> CreditAnimation;
};
