// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/HealingItem.h"
#include "Player/PlayerCharacter.h"

AHealingItem::AHealingItem()
{

}

float AHealingItem::GetHealingPercent() const
{
	return HealingPercent;
}

void AHealingItem::ActivateItem(AActor* Actor)
{
	if (Actor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
		if (Player)
		{
			UE_LOG(LogTemp, Warning, TEXT("Healing Item Activated"));
			//Player->AddHP(Player->MaxHP * HealingPercent / 100.0f);
		}
	}
}
