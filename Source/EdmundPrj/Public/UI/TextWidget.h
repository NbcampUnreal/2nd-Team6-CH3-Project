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
	virtual void PlayAddAnim() override;
	virtual void PlayRemoveAnim() override;

private:
	UFUNCTION()
	void OnClickedNextText();
	
	UFUNCTION()
	void OnClickedSkilButton();

	virtual void EndAddAnim() override;
	virtual void EndRemoveAnim() override;

	void PrintStoryText();
	void CheckCurrentStory();

private:
	UPROPERTY()
	TObjectPtr<UButton> SkipButton;

	UPROPERTY()
	TObjectPtr<UTextBlock> StoryText;
};
