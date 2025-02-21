// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "OptionWidget.generated.h"

class UUIHandle;
class UButton;
class USlider;

UCLASS()
class EDMUNDPRJ_API UOptionWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUIHandle* NewUIHandle) override;
	virtual void EndRemoveAnim() override;

private:
	UFUNCTION()
	void OnClickedCloseOption();

	UFUNCTION()
	void OnClickedBGMVolume(float Value);

	UFUNCTION()
	void OnClickedEffectVolume(float Value);

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveTitleButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveMainButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<USlider> BGMVolumSlider;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<USlider> EffectVolumSlider;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenOptionAnim;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> CloseOptionAnim;
};
