// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "System/EnumSet.h"
#include "SoundHandleSettings.generated.h"


UCLASS(Config = SoundSetting, DefaultConfig, meta = (DisplayName = "SoundHandleSettings"))
class EDMUNDPRJ_API USoundHandleSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Config, Category = "SoundData")
	TSoftObjectPtr<UDataTable> BgmSoundDataTable = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "SoundData")
	TSoftObjectPtr<UDataTable> UISoundDataTable = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "SoundData")
	TSoftObjectPtr<UDataTable> PlayerSoundDataTable = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "SoundData")
	TSoftObjectPtr<UDataTable> MonsterSoundDataTable = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "SoundData")
	TSoftObjectPtr<UDataTable> NpcSoundDataTable = nullptr;

	UPROPERTY(EditAnywhere, Config, Category = "SoundData")
	TSoftObjectPtr<UDataTable> ItemSoundDataTable = nullptr;
};
