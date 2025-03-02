// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/3DWidget/DamageWidget.h"
#include "Components/TextBlock.h"

void UDamageWidget::ApplyDamage(int32 DamageValue)
{
	SetDamageText(DamageValue);
	PlayHitAnim();
}

void UDamageWidget::PlayHitAnim()
{
	if (IsAnimationPlaying(HitAnimation))
	{
		StopAnimation(HitAnimation);
	}

	PlayAnimation(HitAnimation);
}

void UDamageWidget::SetDamageText(int32 DamageValue)
{
	DamageText->SetText(FText::FromString(FString::FromInt(DamageValue)));
}
