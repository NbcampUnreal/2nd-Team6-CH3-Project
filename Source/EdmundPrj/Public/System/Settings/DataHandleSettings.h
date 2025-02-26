// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DataHandleSettings.generated.h"

class UDataTable;

UCLASS(Config = DataSetting, DefaultConfig, meta = (DisplayName = "DataHandleSettings"))
class EDMUNDPRJ_API UDataHandleSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Config, Category = "DataTableAsset")
	TSoftObjectPtr<UDataTable> ShopCatalogDataTable;

	UPROPERTY(EditAnywhere, Config, Category = "DataTableAsset")
	TSoftObjectPtr<UDataTable> PlayDataTable;

	UPROPERTY(EditAnywhere, Config, Category = "DataTableAsset")
	TSoftObjectPtr<UDataTable> PlayerSkillDataTable;

	UPROPERTY(EditAnywhere, Config, Category = "DataTableAsset")
	TSoftObjectPtr<UDataTable> CharacterDataTable;

	UPROPERTY(EditAnywhere, Config, Category = "DataTableAsset")
	TSoftObjectPtr<UDataTable> MissionDataTable;
};
