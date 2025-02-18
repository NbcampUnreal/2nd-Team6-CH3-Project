// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "OptionWidget.generated.h"

class UUIHandle;
class UButton;

UCLASS()
class EDMUNDPRJ_API UOptionWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUIHandle* NewUIHandle) override;

private:
	UFUNCTION()
	void OnClickedMoveToMain();

	UFUNCTION()
	void OnClickedMoveToTitle();

	UFUNCTION()
	void OnClickedQuitGame();

	UFUNCTION()
	void OnClickedCloseOption();

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveTitleButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveMainButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
};
