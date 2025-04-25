// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DataHandle.h"
#include "System/EdmundGameInstance.h"
#include "System/Settings/DataHandleSettings.h"
#include "System/DataStructure/ShopCatalogRow.h"
#include "System/DataStructure/PlayDataRow.h"
#include "System/DataStructure/PlayerSkillRow.h"
#include "System/DataStructure/CharacterDataRow.h"
#include "System/DataStructure/MissionDataRow.h"
#include "System/DataStructure/SpawnerDataRow.h"
#include "System/DataStructure/StoryDataRow.h"
#include "Player/BaseCharacter.h"


void UDataHandle::InitDataHandle(UEdmundGameInstance* NewGameInstance)
{
	EdmundGameInstance = NewGameInstance;

	DataSettings = GetDefault<UDataHandleSettings>();
	checkf(IsValid(DataSettings), TEXT("DataSettings is invalid"));
}

const bool UDataHandle::UpdateCurrentAdvance(const FName& TargetRow, const int32 UpdateValue)
{
	if (!CheckValidOfAdvanceData())
	{
		return false;
	}

	bool bIsPossible = false;

	FShopCatalogRow* ShopCatalog = SelectAdvanceRow(TargetRow);

	if (Consume(ShopCatalog->CurrentPrice))
	{
		ShopCatalog->CurrentLevel += UpdateValue;
		ShopCatalog->CurrentPrice += ShopCatalog->OriginPrice * UpdateValue;
		bIsPossible = true;
	}

	return bIsPossible;
}

const TArray<FShopCatalogRow*>& UDataHandle::GetCurrentAdvance()
{
	CheckValidOfAdvanceData();

	return EdmundDataBuffer.CurrentAdvance;
}

const FShopCatalogRow* UDataHandle::GetCurrentAdvanceByRowName(const FName& TargetRow)
{
	return SelectAdvanceRow(TargetRow);
}

void UDataHandle::AddMoney(const int32 AddValue)
{
	if (!CheckValidOfPlayData())
	{
		return;
	}

	EdmundDataBuffer.PlayData[0]->PossessMoney += AddValue;
}

const int32 UDataHandle::GetMoney()
{
	if (!CheckValidOfPlayData())
	{
		return 0;
	}

	return EdmundDataBuffer.PlayData[0]->PossessMoney;
}

const bool UDataHandle::Consume(const int32 Price)
{
	if (!CheckValidOfPlayData())
	{
		return false;
	}

	if (EdmundDataBuffer.PlayData[0]->PossessMoney < Price)
	{
		return false;
	}

	EdmundDataBuffer.PlayData[0]->PossessMoney -= Price;
	return true;
}

void UDataHandle::SetPlayerType(const ECharacterType CharacterType)
{
	if (!CheckValidOfPlayData())
	{
		return;
	}

	EdmundDataBuffer.PlayData[0]->CharacterType = CharacterType;
}

const ECharacterType UDataHandle::GetPlayerType()
{
	CheckValidOfPlayData();
	return EdmundDataBuffer.PlayData[0]->CharacterType;
}

const TArray<FPlayerSkillRow*>& UDataHandle::GetPlayerSkillData()
{
	CheckValidOfSkillData();
	return EdmundDataBuffer.PlayerSkillData;
}

const TArray<FCharacterDataRow*>& UDataHandle::GetCharacterData()
{
	CheckValidOfCharacterData();
	return EdmundDataBuffer.CharacterData;
}

UClass* UDataHandle::GetCharacterClass()
{
	if (!CheckValidOfPlayData())
	{
		return nullptr;
	}

	if (!CheckValidOfCharacterData())
	{
		return nullptr;
	}

	UClass* CurrentClass = nullptr;

	for (const FCharacterDataRow* CharacterDataRow : EdmundDataBuffer.CharacterData)
	{
		if (CharacterDataRow->CharacterType == EdmundDataBuffer.PlayData[0]->CharacterType)
		{
			CurrentClass = CharacterDataRow->CharacterClass.Get();
		}
	}
	
	return CurrentClass;
}

const TArray<FMissionDataRow*>& UDataHandle::GetMissionDataBySceneType(const ESceneType SceneType)
{
	CheckValidOfMissionData();
	EdmundDataBuffer.CurrentMissionData.Empty();

	for (FMissionDataRow* MissionDataRow : EdmundDataBuffer.MissionData)
	{
		if (MissionDataRow->InSceneType == SceneType)
		{
			EdmundDataBuffer.CurrentMissionData.Add(MissionDataRow);
		}
	}

	return EdmundDataBuffer.CurrentMissionData;
}

const TArray<FSpawnerDataRow*>& UDataHandle::GetSpawnerDataBySceneType(const ESceneType SceneType)
{
	CheckValidOfSpawnData();
	EdmundDataBuffer.CurrentSpawnerData.Empty();

	for (FSpawnerDataRow* SpawnerDataRow : EdmundDataBuffer.SpawnerData)
	{
		if (SpawnerDataRow->InSceneType == SceneType)
		{
			EdmundDataBuffer.CurrentSpawnerData.Add(SpawnerDataRow);
		}
	}
	
	return EdmundDataBuffer.CurrentSpawnerData;
}

const TArray<FStoryDataRow*>& UDataHandle::GetStoryDataBySceneType(const ESceneType SceneType)
{
	CheckValidOfStoryData();
	EdmundDataBuffer.CurrentStoryData.Empty();

	for (FStoryDataRow* StoryDataRow : EdmundDataBuffer.StoryData)
	{
		if (StoryDataRow->SceneType == SceneType)
		{
			EdmundDataBuffer.CurrentStoryData.Add(StoryDataRow);
		}
	}

	return EdmundDataBuffer.CurrentStoryData;
}

void UDataHandle::UpdateClearMission(const ESceneType SceneType)
{
	if (!CheckValidOfPlayData())
	{
		return;
	}

	switch (SceneType)
	{
	case ESceneType::Mission1:
		EdmundDataBuffer.PlayData[0]->bClearedMission1 = true;
		break;

	case ESceneType::Mission2:
		EdmundDataBuffer.PlayData[0]->bClearedMission2 = true;
		break;

	case ESceneType::Mission3:
		EdmundDataBuffer.PlayData[0]->bClearedMission3 = true;
		break;

	default:
		checkNoEntry();
		break;
	}
}

void UDataHandle::UpdateShowedIntro(const bool bShowed)
{
	if (!CheckValidOfPlayData())
	{
		return;
	}

	EdmundDataBuffer.PlayData[0]->bShowedIntro = bShowed;
}

const bool UDataHandle::GetIsShowedIntro()
{
	if (!CheckValidOfPlayData())
	{
		return false;
	}

	return EdmundDataBuffer.PlayData[0]->bShowedIntro;
}

const bool UDataHandle::GetIsClearedMission(const int32 Index)
{
	if (!CheckValidOfPlayData())
	{
		return false;
	}

	switch (Index)
	{
	case 0:
		return EdmundDataBuffer.PlayData[0]->bClearedMission1;
		break;

	case 1:
		return EdmundDataBuffer.PlayData[0]->bClearedMission2;
		break;

	case 2:
		return EdmundDataBuffer.PlayData[0]->bClearedMission3;
		break;

	default:
		checkNoEntry();
		break;
	}

	return false;
}

FShopCatalogRow* UDataHandle::SelectAdvanceRow(const FName& TargetRow)
{
	if (!CheckValidOfAdvanceData())
	{
		return nullptr;
	}

	FShopCatalogRow* Result = nullptr;

	for (FShopCatalogRow* ShopCatalog : EdmundDataBuffer.CurrentAdvance)
	{
		if (ShopCatalog->AdvanceName == TargetRow)
		{
			Result = ShopCatalog;
		}
	}
	return Result;
}

bool UDataHandle::CheckValidOfAdvanceData()
{
	if (EdmundDataBuffer.CurrentAdvance.IsEmpty())
	{
		if (!IsValid(DataSettings))
		{
			return false;
		}

		if (DataSettings->ShopCatalogDataTable.IsNull())
		{
			return false;
		}

		const FString DataContext(TEXT("Advance Data ConText"));

		ShopCatalogDataTable = DataSettings->ShopCatalogDataTable.LoadSynchronous();
		ShopCatalogDataTable->GetAllRows(DataContext, EdmundDataBuffer.CurrentAdvance);
	}

	return true;
}

bool UDataHandle::CheckValidOfPlayData()
{
	if (EdmundDataBuffer.PlayData.IsEmpty())
	{
		if (!IsValid(DataSettings))
		{
			return false;
		}

		if (DataSettings->PlayDataTable.IsNull())
		{
			return false;
		}

		const FString DataContext(TEXT("Play Data ConText"));

		PlayDataTable = DataSettings->PlayDataTable.LoadSynchronous();
		PlayDataTable->GetAllRows(DataContext, EdmundDataBuffer.PlayData);
	}

	return true;
}

bool UDataHandle::CheckValidOfSkillData()
{
	if (EdmundDataBuffer.PlayerSkillData.IsEmpty())
	{
		if (!IsValid(DataSettings))
		{
			return false;
		}

		if (DataSettings->PlayerSkillDataTable.IsNull())
		{
			return false;
		}

		const FString DataContext(TEXT("Skill Data ConText"));

		PlayerSkillDataTable = DataSettings->PlayerSkillDataTable.LoadSynchronous();
		PlayerSkillDataTable->GetAllRows(DataContext, EdmundDataBuffer.PlayerSkillData);
	}

	return true;
}

bool UDataHandle::CheckValidOfCharacterData()
{
	if (EdmundDataBuffer.CharacterData.IsEmpty())
	{
		if (!IsValid(DataSettings))
		{
			return false;
		}

		if (DataSettings->CharacterDataTable.IsNull())
		{
			return false;
		}

		const FString DataContext(TEXT("Character Data ConText"));

		CharacterDataTable = DataSettings->CharacterDataTable.LoadSynchronous();
		CharacterDataTable->GetAllRows(DataContext, EdmundDataBuffer.CharacterData);
	}

	return true;
}

bool UDataHandle::CheckValidOfMissionData()
{
	if (EdmundDataBuffer.MissionData.IsEmpty())
	{
		if (!IsValid(DataSettings))
		{
			return false;
		}

		if (DataSettings->MissionDataTable.IsNull())
		{
			return false;
		}

		const FString DataContext(TEXT("Mission Data ConText"));

		MissionDataTable = DataSettings->MissionDataTable.LoadSynchronous();
		MissionDataTable->GetAllRows(DataContext, EdmundDataBuffer.MissionData);
	}

	return true;
}

bool UDataHandle::CheckValidOfSpawnData()
{
	if (EdmundDataBuffer.SpawnerData.IsEmpty())
	{
		if (!IsValid(DataSettings))
		{
			return false;
		}

		if (DataSettings->SpawnerDataTable.IsNull())
		{
			return false;
		}

		const FString DataContext(TEXT("Spawn Data ConText"));

		SpawnerDataTable = DataSettings->SpawnerDataTable.LoadSynchronous();
		SpawnerDataTable->GetAllRows(DataContext, EdmundDataBuffer.SpawnerData);
	}

	return true;
}

bool UDataHandle::CheckValidOfStoryData()
{
	if (EdmundDataBuffer.StoryData.IsEmpty())
	{
		if (!IsValid(DataSettings))
		{
			return false;
		}

		if (DataSettings->StoryDataTable.IsNull())
		{
			return false;
		}

		const FString DataContext(TEXT("Story Data ConText"));

		StoryDataTable = DataSettings->StoryDataTable.LoadSynchronous();
		StoryDataTable->GetAllRows(DataContext, EdmundDataBuffer.StoryData);
	}

	return true;
}
