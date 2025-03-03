// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "TextWidget.generated.h"

class UButton;
class UTextBlock;
class UUIHandle;

UCLASS()
class EDMUNDPRJ_API UTextWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUIHandle* NewUIHandle) override;
	virtual void ChangedStoryText(const FString& TargetText) override;
	
private:
	UFUNCTION()
	void OnClickedNextText();
	
	UFUNCTION()
	void OnClickedSkipButton();

	virtual void EndRemoveAnim() override;

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> SkipButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> StoryText;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenAnim;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> CloseAnim;
};
