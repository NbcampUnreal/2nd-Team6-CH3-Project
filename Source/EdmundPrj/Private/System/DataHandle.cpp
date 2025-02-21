// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DataHandle.h"
#include "System/EdmundGameInstance.h"
#include "System/Settings/DataHandleSettings.h"


void UDataHandle::InitDataHandle(UEdmundGameInstance* NewGameInstance)
{
	EdmundGameInstance = NewGameInstance;

	const UDataHandleSettings* DataSettings = GetDefault<UDataHandleSettings>();
	checkf(IsValid(DataSettings), TEXT("DataSettings is invalid"));

	LoadDataTables(DataSettings);
}

void UDataHandle::LoadDataTables(const UDataHandleSettings* DataSettings)
{
	ShopCatalogDataTable = DataSettings->ShopCatalogDataTable.LoadSynchronous();
}
