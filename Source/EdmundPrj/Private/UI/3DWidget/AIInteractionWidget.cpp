// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/3DWidget/AIInteractionWidget.h"
#include "Components/ProgressBar.h"
#include "UI/3DWidget/DamageWidget.h"
#include "Components/CanvasPanel.h"

void UAIInteractionWidget::InitWidget()
{
	DamageWidgetSet.Add(DamageWidget0);
	DamageWidgetSet.Add(DamageWidget1);
	DamageWidgetSet.Add(DamageWidget2);
}

void UAIInteractionWidget::ApplyHitEvent(int32 MaxHp, int32 CurrentHp, int32 Damage)
{
	UpdateHpBar(MaxHp, CurrentHp);
	UpdateDamageText(Damage);
}

void UAIInteractionWidget::SetIsVisible(bool IsVisible)
{
	if (IsVisible)
	{
		TotalPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TotalPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAIInteractionWidget::UpdateHpBar(int32 MaxHp, int32 CurrentHp)
{
	HpBar->SetPercent((float)CurrentHp / (float)MaxHp);

	if (IsAnimationPlaying(HpAnimation))
	{
		StopAnimation(HpAnimation);
	}

	PlayAnimation(HpAnimation);
}

void UAIInteractionWidget::UpdateDamageText(int32 Damage)
{
	if (DamageWidgetSet.Num() == 0)
	{
		return;
	}

	DamageIndex = DamageIndex % DamageWidgetSet.Num();

	DamageWidgetSet[DamageIndex]->ApplyDamage(Damage);

	++DamageIndex;
}

