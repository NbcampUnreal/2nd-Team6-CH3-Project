// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/3DWidget/InteractionWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"

void UInteractionWidget::UpdateProgressBar(float Value)
{
	ProgressBar->SetPercent(Value);
}

void UInteractionWidget::UpdateKeyText(FString InputKeyString)
{
	KeyText->SetText(FText::FromString(InputKeyString));
}

void UInteractionWidget::UpdateInfoText(FString KeyInfoString)
{
	InfoText->SetText(FText::FromString(KeyInfoString));
}

void UInteractionWidget::VisibleNotify(bool bIsVisible)
{
	if (bIsVisible)
	{
		NotifyBox->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		NotifyBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInteractionWidget::VisibleProgressBar(bool bIsVisible)
{
	if (bIsVisible)
	{
		ProgressBox->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ProgressBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}
