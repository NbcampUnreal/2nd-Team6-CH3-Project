// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GoldItem.h"

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
	UE_LOG(LogTemp, Warning, TEXT("골드획득"));
	PlaySound();
}
