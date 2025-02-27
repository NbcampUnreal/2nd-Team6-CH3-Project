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
	GameState->AddCurrentLevelMoney(GoldAmount);
	UE_LOG(LogTemp, Warning, TEXT("%f 골드획득"), GoldAmount);
	PlaySound();
}
