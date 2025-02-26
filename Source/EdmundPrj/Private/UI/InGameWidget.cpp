// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UInGameWidget::ChangedNotifyText(const FString& NotifyText)
{
	if (IsAnimationPlaying(NotifyAnimation))
	{
		StopAnimation(NotifyAnimation);
	}

	NotifyTextBlock->SetText(FText::FromString(NotifyText));
	PlayAnimation(NotifyAnimation);
}

void UInGameWidget::ChangedMissionText(const FString& MissionText)
{
	UE_LOG(LogTemp, Warning, TEXT("In Mission Text Notify"));
	MissionInfoText->SetText(FText::FromString(MissionText));
}

void UInGameWidget::ChangedPlayerHp(const int32 MaxHp, const int32 CurrentHp)
{
	HpText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentHp, MaxHp)));
	HpBar->SetPercent((float)CurrentHp / (float)MaxHp);
}

void UInGameWidget::ChangedPlayerOther(const int32 MaxValue, const int32 CurrentValue)
{
	HpText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentValue, MaxValue)));
	OtherBar->SetPercent((float)CurrentValue / (float)MaxValue);
}

void UInGameWidget::ChangedPlayerAmmo(const int32 MaxAmmo, const int32 CurrentAmmo)
{
	HpText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo)));
}



