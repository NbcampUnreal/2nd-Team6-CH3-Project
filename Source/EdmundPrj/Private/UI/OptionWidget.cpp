// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionWidget.h"
#include "System/UIHandle.h"
#include "Components/Button.h"
#include "Components/Slider.h"

void UOptionWidget::InitWidget(UUIHandle* NewUIHandle)
{
	OpenAnimation = OpenOptionAnim;
	CloseAnimation = CloseOptionAnim;

	Super::InitWidget(NewUIHandle);

	MoveTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTitle);
	MoveMainButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveMain);
	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCloseOption);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedQuitGame);
	BGMVolumSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnClickedBGMVolume);
	EffectVolumSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnClickedEffectVolume);

	checkf(IsValid(UIHandle), TEXT("UIHandle is not valid"));

	BGMVolumSlider->SetValue(UIHandle->GetBGMVolumeByGameInstance());
	EffectVolumSlider->SetValue(UIHandle->GetEffectVolumeByGameInstance());
}

void UOptionWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();

	UIHandle->RemoveCoverFromViewport(EWidgetType::OptionWidget);
}

void UOptionWidget::OnClickedCloseOption()
{
	OnClickedCloseWidget(EWidgetType::OptionWidget);
}

void UOptionWidget::OnClickedBGMVolume(float Value)
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedBGMVolume(Value);
}

void UOptionWidget::OnClickedEffectVolume(float Value)
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedEffectVolume(Value);
}
