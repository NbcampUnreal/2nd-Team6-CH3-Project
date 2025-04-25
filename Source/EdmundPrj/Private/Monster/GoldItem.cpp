// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GoldItem.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "System/EdmundGameState.h"

AGoldItem::AGoldItem()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController))
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (IsValid(PlayerPawn))
		{
			ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(PlayerPawn);
			if (PlayerCharacter)
			{
				GoldAmount += GoldAmount * (PlayerCharacter->GetGoldMultipler() - 100) / 100;
			}
		}
	}

	ItemType = EItemType::Gold;
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
	if (GameState)
	{
		GameState->AddCurrentLevelMoney(GoldAmount);
	}

	PlaySound();
}
