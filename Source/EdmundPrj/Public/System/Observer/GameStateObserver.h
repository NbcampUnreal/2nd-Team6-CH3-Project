// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "System/EnumSet.h"
#include "GameStateObserver.generated.h"

UINTERFACE(MinimalAPI)
class UGameStateObserver : public UInterface
{
	GENERATED_BODY()
};


class EDMUNDPRJ_API IGameStateObserver
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void ChangedPlayerHp(const int32 MaxHp, const int32 CurrentHp) = 0;

	UFUNCTION()
	virtual void ChangedPlayerAmmo(const int32 MaxAmmo, const int32 CurrentAmmo) = 0;

	UFUNCTION()
	virtual void ChangedPlayerOther(const int32 MaxValue, const int32 CurrentValue) = 0;

	UFUNCTION()
	virtual void ChangedPlayerMoney(const int32 Money) = 0;

	UFUNCTION()
	virtual void ChangedPlayerExp(const int32 MaxExp, const int32 CurrentExp) = 0;

	UFUNCTION()
	virtual void ChangedSkillList() = 0;

	UFUNCTION()
	virtual void ChangedCharacterType(const ECharacterType CharacterType) = 0;

	UFUNCTION()
	virtual void ChangedNotifyText(const FString& NotifyText) = 0;

	UFUNCTION()
	virtual void ChangedMissionText(const FString& MissionText) = 0;
};
