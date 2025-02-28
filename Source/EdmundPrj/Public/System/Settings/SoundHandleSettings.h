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
	TSoftObjectPtr<UDataTable> BgmSoundDataTable;

	UPROPERTY(EditAnywhere, Config, Category = "SoundData")
	TSoftObjectPtr<UDataTable> UISoundDataTable;

	UPROPERTY(EditAnywhere, Config, Category = "SoundData")
	TSoftObjectPtr<UDataTable> PlayerSoundDataTable;

	UPROPERTY(EditAnywhere, Config, Category = "SoundData")
	TSoftObjectPtr<UDataTable> MonsterSoundDataTable;

	UPROPERTY(EditAnywhere, Config, Category = "SoundData")
	TSoftObjectPtr<UDataTable> NpcSoundDataTable;

	UPROPERTY(EditAnywhere, Config, Category = "SoundData")
	TSoftObjectPtr<UDataTable> ItemSoundDataTable;
};
