// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "System/Observer/GameStateObserver.h"
#include "BaseWidget.generated.h"

class UUIHandle;

UCLASS()
class EDMUNDPRJ_API UBaseWidget : public UUserWidget, public IGameStateObserver
{
	GENERATED_BODY()

public:
	virtual void ChangedPlayerMaxHp(int32 Hp) override;
	virtual void ChangedPlayerCurrentHp(int32 Hp) override;
	virtual void ChangedPlayerMaxAmmo(int32 Ammo) override;
	virtual void ChangedPlayerAmmo(int32 Ammo) override;
	virtual void ChangedPlayerMoney(int32 Money) override;
	virtual void InitWidget(UUIHandle* uiHandle);
	virtual void Action();
	virtual void Update();

protected:
	TObjectPtr<UUIHandle> UIHandle;
};
