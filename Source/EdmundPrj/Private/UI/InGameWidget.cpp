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
	MissionInfoText->SetText(FText::FromString(MissionText));

	bOnMissionInfo = true;
	PlayAnimationReverse(MissionInfoAnimation);
}

void UInGameWidget::ChangedPlayerHp(const int32 MaxHp, const int32 CurrentHp)
{
	HpText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentHp, MaxHp)));
	HpBar->SetPercent((float)CurrentHp / (float)MaxHp);
}

void UInGameWidget::ChangedPlayerOther(const int32 MaxValue, const int32 CurrentValue)
{
	OtherText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentValue, MaxValue)));
	OtherBar->SetPercent((float)CurrentValue / (float)MaxValue);
}

void UInGameWidget::ChangedPlayerAmmo(const int32 MaxAmmo, const int32 CurrentAmmo)
{
	AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo)));
}

void UInGameWidget::ChangedPlayerExp(const int32 MaxExp, const int32 CurrentExp)
{
	if (bIsMaxLevel)
	{
		ExpBar->SetPercent(1.0f);
	}
	else
	{
		ExpBar->SetPercent((float)CurrentExp / (float)MaxExp);
	}
}

void UInGameWidget::ChangedPlayerLevel(const int32 LevelValue)
{
	LevelText->SetText(FText::FromString(FString::FromInt(LevelValue)));

	if (LevelValue == 30)
	{
		bIsMaxLevel = true;
	}
}

void UInGameWidget::ChangedCharacterType(const ECharacterType CharacterType)
{
	switch (CharacterType)
	{
	case ECharacterType::Gunner:
		SetAmmoTextVisibility(true);
		break;

	case ECharacterType::Aurora:
		SetAmmoTextVisibility(false);
		break;

	case ECharacterType::Fey:
		SetAmmoTextVisibility(false);
		break;

	case ECharacterType::Sparrow:
		SetAmmoTextVisibility(true);
		break;

	default:
		checkNoEntry();
		break;
	}
}

void UInGameWidget::ChangedStageToProgress(const FString& ProgressText, const bool bIsOn)
{
	BossHpBarText->SetText(FText::FromString(ProgressText));

	if (bIsOn)
	{
		PlayAnimation(BossHpAnimation);
	}
	else
	{
		PlayAnimationReverse(BossHpAnimation);
	}
}

void UInGameWidget::ChangedBossHp(const int32 MaxHp, const int32 CurrentHp)
{
	BossHpBar->SetPercent((float)CurrentHp / (float)MaxHp);
}

void UInGameWidget::ChangedMissionInfoOnOff()
{
	if (bOnMissionInfo)
	{
		bOnMissionInfo = false;
		PlayAnimation(MissionInfoAnimation);
	}
	else
	{
		bOnMissionInfo = true;
		PlayAnimationReverse(MissionInfoAnimation);
	}
}

void UInGameWidget::SetAmmoTextVisibility(bool bIsVisible)
{
	if (bIsVisible)
	{
		AmmoText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		AmmoText->SetVisibility(ESlateVisibility::Collapsed);
	}
}
