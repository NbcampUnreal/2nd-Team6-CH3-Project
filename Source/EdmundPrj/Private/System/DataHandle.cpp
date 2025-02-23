// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DataHandle.h"
#include "System/EdmundGameInstance.h"
#include "System/Settings/DataHandleSettings.h"
#include "System/DataStructure/ShopCatalogRow.h"
#include "System/DataStructure/PlayDataRow.h"


void UDataHandle::InitDataHandle(UEdmundGameInstance* NewGameInstance)
{
	EdmundGameInstance = NewGameInstance;

	const UDataHandleSettings* DataSettings = GetDefault<UDataHandleSettings>();
	checkf(IsValid(DataSettings), TEXT("DataSettings is invalid"));

	LoadDataTables(DataSettings);
}

const bool UDataHandle::UpdateCurrentAdvance(const FName& TargetRow, const int32 UpdateValue)
{
	bool bIsPossible = false;

	for (FShopCatalogRow* ShopCatalog : CurrentAdvance)
	{
		if (ShopCatalog->AdvanceName == TargetRow)
		{
			if (Consume(ShopCatalog->CurrentPrice))
			{
				ShopCatalog->CurrentLevel += UpdateValue;
				ShopCatalog->CurrentPrice += ShopCatalog->OriginPrice * UpdateValue;
				bIsPossible = true;
				break;
			}
		}
	}

	return bIsPossible;
}

const TArray<FShopCatalogRow*>& UDataHandle::GetCurrentAdvance() const
{
	return CurrentAdvance;
}

const FShopCatalogRow* UDataHandle::GetCurrentAdvanceByRowName(const FName& TargetRow) const
{
	FShopCatalogRow* Result = nullptr;

	for (FShopCatalogRow* ShopCatalog : CurrentAdvance)
	{
		if (ShopCatalog->AdvanceName == TargetRow)
		{
			Result = ShopCatalog;
		}
	}
	return Result;
}

void UDataHandle::AddMoney(const int32 AddValue)
{
	PlayData[0]->PossessMoney += AddValue;
}

const int32 UDataHandle::GetMoney() const
{
	return PlayData[0]->PossessMoney;
}

const bool UDataHandle::Consume(const int32 Price)
{
	if (PlayData[0]->PossessMoney < Price)
	{
		return false;
	}

	PlayData[0]->PossessMoney -= Price;
	return true;
}

void UDataHandle::SetPlayerType(const ECharacterType CharacterType)
{
	PlayData[0]->CharacterType = CharacterType;
}

const ECharacterType UDataHandle::GetPlayerType() const
{
	return PlayData[0]->CharacterType;
}

void UDataHandle::UpdateClearMission(const int32 Index)
{
	switch (Index)
	{
	case 0:
		PlayData[0]->bClearedMission1 = true;
		break;

	case 1:
		PlayData[0]->bClearedMission2 = true;
		break;

	case 2:
		PlayData[0]->bClearedMission3 = true;
		break;

	default:
		checkNoEntry();
		break;
	}
}

void UDataHandle::UpdateShowedIntro(const bool bShowed)
{
	PlayData[0]->bShowedIntro = bShowed;
}

const bool UDataHandle::IsShowedIntro() const
{
	return PlayData[0]->bShowedIntro;
}

const bool UDataHandle::IsClearedMission(const int32 Index) const
{
	switch (Index)
	{
	case 0:
		return PlayData[0]->bClearedMission1;
		break;

	case 1:
		return PlayData[0]->bClearedMission2;
		break;

	case 2:
		return PlayData[0]->bClearedMission3;
		break;

	default:
		checkNoEntry();
		break;
	}

	return false;
}

const UDataTable* UDataHandle::GetDataTable(const ETableType TableType) const
{
	switch (TableType)
	{
	case ETableType::MissionItem:

		break;

	case ETableType::PlayerSkill:

		break;

	case ETableType::ShopCatalog:
		return ShopCatalogDataTable;
		break;

	default:
		return nullptr;
		break;
	}

	return nullptr;
}

void UDataHandle::LoadDataTables(const UDataHandleSettings* DataSettings)
{
	ShopCatalogDataTable = DataSettings->ShopCatalogDataTable.LoadSynchronous();
	PlayDataTable = DataSettings->PlayDataTable.LoadSynchronous();

	const FString DataContext(TEXT("Data ConText"));

	ShopCatalogDataTable->GetAllRows(DataContext, CurrentAdvance);
	PlayDataTable->GetAllRows(DataContext, PlayData);

	UE_LOG(LogTemp, Warning, TEXT("Loaded Data"));
}
