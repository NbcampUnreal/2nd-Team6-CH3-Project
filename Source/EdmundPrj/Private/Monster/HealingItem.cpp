// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/HealingItem.h"
#include "Player/BaseCharacter.h"

AHealingItem::AHealingItem()
{

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
			UE_LOG(LogTemp, Warning, TEXT("Healing Item Activated"));
			//Player->(Player->MaxHP * HealingPercent / 100.0f);
			PlaySound();
		}
	}
}