// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GoldItem.h"
#include "System/EdmundGameState.h"

AGoldItem::AGoldItem()
{
}

float AGoldItem::GetGoldAmount() const
{
	return GoldAmount;
}

void AGoldItem::SetGoldAmount(float NewGold)
{
	GoldAmount = NewGold;
}

void AGoldItem::ActivateItem(AActor* Actor)
{
	AEdmundGameState* GameState = GetWorld()->GetGameState<AEdmundGameState>();

	if (GameState)
	{
		GameState->AddCurrentLevelMoney(GoldAmount);
		UE_LOG(LogTemp, Warning, TEXT("%f 골드획득"), GoldAmount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameState가 없습니다."));
	}

	PlaySound();
}
