// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionWidget.h"
#include "System/UIHandle.h"
#include "Components/Button.h"
#include "Components/Slider.h"

void UOptionWidget::InitWidget(UUIHandle* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	MoveTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveToTitle);
	MoveMainButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveToMain);
	CloseButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedCloseOption);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedQuitGame);
	BGMVolumSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnClickedBGMVolume);
	EffectVolumSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnClickedEffectVolume);

	BindToAnimationStarted(OpenOptionAnim, StartAddDelegate);
	BindToAnimationFinished(OpenOptionAnim, EndAddDelegate);
	BindToAnimationStarted(CloseOptionAnim, StartRemoveDelegate);
	BindToAnimationFinished(CloseOptionAnim, EndRemoveDelegate);

	checkf(IsValid(UIHandle), TEXT("UIHandle is not valid"));

	BGMVolumSlider->SetValue(UIHandle->GetBGMVolumeByGameInstance());
	EffectVolumSlider->SetValue(UIHandle->GetEffectVolumeByGameInstance());
}

void UOptionWidget::PlayAddAnim()
{
	Super::PlayAddAnim();

	if (bIsPlaying)
	{
		return;
	}

	PlayAnimation(OpenOptionAnim);
}

void UOptionWidget::PlayRemoveAnim(bool bIsNext, ESceneType SceneType)
{
	Super::PlayRemoveAnim();

	if (bIsPlaying)
	{
		return;
	}

	PlayAnimation(CloseOptionAnim);
}

void UOptionWidget::EndRemoveAnim()
{
	Super::EndRemoveAnim();

	UIHandle->RemoveCoverFromViewport(EWidgetType::OptionWidget);
}

void UOptionWidget::OnClickedMoveToMain()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	PlayRemoveAnim();
	UIHandle->ClickedMoveToMain();
}

void UOptionWidget::OnClickedMoveToTitle()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	PlayRemoveAnim();
	UIHandle->ClickedMoveToTitle();
}

void UOptionWidget::OnClickedQuitGame()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->ClickedQuitGame();
}

void UOptionWidget::OnClickedCloseOption()
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->RequestRemoveCoverFromViewport(EWidgetType::OptionWidget);
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
