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
	virtual void ChangedPlayerMaxHp(int32 Hp) = 0;

	UFUNCTION()
	virtual void ChangedPlayerCurrentHp(int32 Hp) = 0;

	UFUNCTION()
	virtual void ChangedPlayerMaxAmmo(int32 Ammo) = 0;

	UFUNCTION()
	virtual void ChangedPlayerAmmo(int32 Ammo) = 0;

	UFUNCTION()
	virtual void ChangedPlayerMoney(int32 Money) = 0;

	UFUNCTION()
	virtual void ChangedSkillList() = 0;

	UFUNCTION()
	virtual void ChangedCharacterType(ECharacterType CharacterType) = 0;
};
