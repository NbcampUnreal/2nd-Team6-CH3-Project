// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/HealingItem.h"
#include "Player/BaseCharacter.h"

AHealingItem::AHealingItem()
{
	ItemType = EItemType::HealKit;
}

float AHealingItem::GetHealingPercent() const
{
	return HealingPercent;
}

void AHealingItem::SetHealingPercent(float NewPercent)
{
	HealingPercent = NewPercent;
}


void AHealingItem::ActivateItem(AActor* Actor)
{
	if (Actor)
	{
		ABaseCharacter* Player = Cast<ABaseCharacter>(Actor);
		if (Player)
		{
			float HealAmount = Player->GetMaxHP() * HealingPercent / 100.0f;

			Player->AmountHP(HealAmount);
			PlaySound();
		}
	}
}