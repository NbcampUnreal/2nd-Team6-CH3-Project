// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SoundHandle.h"
#include "System/EdmundGameInstance.h"
#include "System/Settings/SoundHandleSettings.h"
#include "System/DataStructure/BgmDataRow.h"
#include "System/DataStructure/UISoundDataRow.h"
#include "System/DataStructure/PlayerSoundDataRow.h"
#include "System/DataStructure/MonsterSoundDataRow.h"
#include "System/DataStructure/NpcSoundDataRow.h"
#include "System/DataStructure/ItemSoundDataRow.h"
#include "Components/AudioComponent.h"
#include <Kismet/GameplayStatics.h>
#include "System/Struct/SoundSourceData.h"

void USoundHandle::InitSoundHandle(UEdmundGameInstance* NewGameInstance)
{
	EdmundGameInstance = NewGameInstance;

	SoundSettings = GetDefault<USoundHandleSettings>();
	checkf(IsValid(SoundSettings), TEXT("SoundSetting is invalid"));
}

void USoundHandle::PlayBgmBySceneType(ESceneType SceneType)
{
	EBGMSoundType BgmType = EBGMSoundType::Title;

	switch (SceneType)
	{
	case ESceneType::Title:
		BgmType = EBGMSoundType::Title;
		break;

	case ESceneType::Main:
		BgmType = EBGMSoundType::Main;
		break;

	case ESceneType::Mission1:
		BgmType = EBGMSoundType::Mission1;
		break;

	case ESceneType::Mission2:
		BgmType = EBGMSoundType::Mission2;
		break;

	case ESceneType::Mission3:
		BgmType = EBGMSoundType::Mission3;
		break;

	case ESceneType::Infinity:
		BgmType = EBGMSoundType::Infinity;
		break;

	case ESceneType::Ending:
		BgmType = EBGMSoundType::Ending;
		break;

	default:
		checkNoEntry();
		break;
	}

	PlayBgmByBgmType(BgmType);
}

void USoundHandle::PlayBgmByBgmType(EBGMSoundType BgmSoundType)
{
	if (!CheckValidOfBgmSource(BgmSoundType))
	{
		return;
	}

	if (!IsValid(BgmSet[BgmSoundType]))
	{
		return;
	}

	if (!IsValid(BgmAudio))
	{
		BgmAudio = UGameplayStatics::CreateSound2D(EdmundGameInstance, BgmSet[BgmSoundType], BGMVolume);
		BgmAudio->bAutoDestroy = false;
	}

	if (BgmAudio->IsPlaying())
	{
		BgmAudio->Stop();
	}

	BgmAudio->SetSound(BgmSet[BgmSoundType]);
	BgmAudio->Play();
}

void USoundHandle::PlayUISound(EUISoundType UISoundType)
{
	if (!CheckValidOfUISoundSource(UISoundType))
	{
		return;
	}

	if (!IsValid(UISet[UISoundType]))
	{
		return;
	}

	if (!IsValid(UIAudio))
	{
		UIAudio = UGameplayStatics::CreateSound2D(EdmundGameInstance, UISet[UISoundType], EffectVolume);
		UIAudio->bAutoDestroy = false;
	}

	if (UIAudio->IsPlaying())
	{
		UIAudio->Stop();
	}

	UIAudio->SetSound(UISet[UISoundType]);
	UIAudio->Play();
}

USoundBase* USoundHandle::GetPlayerSound(ESoundType SoundType)
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));

	ECharacterType CharacterType = EdmundGameInstance->GetPlayerType();

	if (!CheckValidOfPlayerSoundSource(CharacterType, SoundType))
	{
		return nullptr;
	}

	return SoundBuffer.PlayerSoundMap[CharacterType].SoundMap[SoundType];
}

USoundBase* USoundHandle::GetMonsterSound(EMonsterType MonsterType, ESoundType SoundType)
{
	if (!CheckValidOfMonsterSoundSource(MonsterType, SoundType))
	{
		return nullptr;
	}

	return SoundBuffer.MonsterSoundMap[MonsterType].SoundMap[SoundType];
}

USoundBase* USoundHandle::GetNpcSound(ENpcType NpcType, ESoundType SoundType)
{
	if (!CheckValidOfNpcSoundSource(NpcType, SoundType))
	{
		return nullptr;
	}

	return SoundBuffer.NpcSoundMap[NpcType].SoundMap[SoundType];
}

USoundBase* USoundHandle::GetItemrSound(EItemType ItemType, ESoundType SoundType)
{
	if (!CheckValidOfItemSoundSource(ItemType, SoundType))
	{
		return nullptr;
	}

	return SoundBuffer.ItemSoundMap[ItemType].SoundMap[SoundType];
}

void USoundHandle::UpdateBGMVolume(const float VolumeValue)
{
	BGMVolume = VolumeValue;
	
	if (BGMVolume <= 0)
	{
		BGMVolume = 0;
	}

	if (IsValid(BgmAudio))
	{
		BgmAudio->SetVolumeMultiplier(VolumeValue);
	}
}

void USoundHandle::UpdateEffectVolume(const float VolumeValue)
{
	EffectVolume = VolumeValue;

	if (IsValid(UIAudio))
	{
		UIAudio->SetVolumeMultiplier(VolumeValue);
	}
}

float USoundHandle::GetBGMVolume() const
{
	return BGMVolume;
}

float USoundHandle::GetEffectVolume() const
{
	return EffectVolume;
}

bool USoundHandle::CheckValidOfBgmSource(EBGMSoundType Type)
{
	if (BgmSet.Contains(Type))
	{
		return true;
	}

	if (!CheckValidOfBgmData())
	{
		return false;
	}

	USoundBase* SoundSource = nullptr;

	for (const FBgmDataRow& SoundData : SoundBuffer.BgmData)
	{
		if (SoundData.Type == Type)
		{
			SoundSource = SoundData.SoundSource.LoadSynchronous();
			break;
		}
	}

	if (!IsValid(SoundSource))
	{
		return false;
	}

	BgmSet.Add(Type, SoundSource);

	return true;
}

bool USoundHandle::CheckValidOfBgmData()
{
	if (SoundBuffer.BgmData.IsEmpty())
	{
		if (!IsValid(SoundSettings))
		{
			return false;
		}

		if (SoundSettings->BgmSoundDataTable.IsNull())
		{
			return false;
		}

		TArray<FBgmDataRow*> TempArray;
		const FString DataContext(TEXT("Bgm Data Load"));

		BgmDataTable = SoundSettings->BgmSoundDataTable.LoadSynchronous();
		BgmDataTable->GetAllRows(DataContext, TempArray);

		for (const FBgmDataRow* BgmDataRow : TempArray)
		{
			SoundBuffer.BgmData.Add(*BgmDataRow);
		}
	}

	return true;
}

bool USoundHandle::CheckValidOfUISoundSource(EUISoundType Type)
{
	if (UISet.Contains(Type))
	{
		return true;
	}

	if (!CheckValidOfUISoundData())
	{
		return false;
	}

	USoundBase* SoundSource = nullptr;

	for (const FUISoundDataRow& SoundData : SoundBuffer.UIData)
	{
		if (SoundData.Type == Type)
		{
			SoundSource = SoundData.SoundSource.LoadSynchronous();
			break;
		}
	}

	if (!IsValid(SoundSource))
	{
		return false;
	}

	UISet.Add(Type, SoundSource);

	return true;
}

bool USoundHandle::CheckValidOfUISoundData()
{
	if (SoundBuffer.UIData.IsEmpty())
	{
		if (!IsValid(SoundSettings))
		{
			return false;
		}

		if (SoundSettings->UISoundDataTable.IsNull())
		{
			return false;
		}

		TArray<FUISoundDataRow*> TempArray;
		const FString DataContext(TEXT("UI Data Load"));

		UIDataTable = SoundSettings->UISoundDataTable.LoadSynchronous();
		UIDataTable->GetAllRows(DataContext, TempArray);

		for (const FUISoundDataRow* UISoundDataRow : TempArray)
		{
			SoundBuffer.UIData.Add(*UISoundDataRow);
		}
	}

	return true;
}

bool USoundHandle::CheckValidOfPlayerSoundSource(ECharacterType CharacterType, ESoundType SoundType)
{
	if (SoundBuffer.PlayerSoundMap.Contains(CharacterType))
	{
		if (SoundBuffer.PlayerSoundMap[CharacterType].SoundMap.Contains(SoundType))
		{
			return true;
		}
	}

	if (!CheckValidOfPlayerSoundData())
	{
		return false;
	}

	USoundBase* SoundSource = nullptr;

	for (const FPlayerSoundDataRow& SoundData : SoundBuffer.PlayerData)
	{
		if (SoundData.CharacterType == CharacterType)
		{
			if (SoundData.SoundType == SoundType)
			{
				SoundSource = SoundData.SoundSource.LoadSynchronous();
				break;
			}
		}
	}

	if (!IsValid(SoundSource))
	{
		return false;
	}

	FSoundSourceData TempData;
	TempData.SoundMap.Add(SoundType, SoundSource);
	SoundBuffer.PlayerSoundMap.Add(CharacterType, TempData);

	return true;
}

bool USoundHandle::CheckValidOfPlayerSoundData()
{
	if (SoundBuffer.PlayerData.IsEmpty())
	{
		if (!IsValid(SoundSettings))
		{
			return false;
		}

		if (SoundSettings->PlayerSoundDataTable.IsNull())
		{
			return false;
		}

		TArray<FPlayerSoundDataRow*> TempArray;
		const FString DataContext(TEXT("Player Data Load"));

		PlayerDataTable = SoundSettings->PlayerSoundDataTable.LoadSynchronous();
		PlayerDataTable->GetAllRows(DataContext, TempArray);

		for (const FPlayerSoundDataRow* SoundDataRow : TempArray)
		{
			SoundBuffer.PlayerData.Add(*SoundDataRow);
		}
	}

	return true;
}

bool USoundHandle::CheckValidOfMonsterSoundSource(EMonsterType MonsterType, ESoundType SoundType)
{
	if (SoundBuffer.MonsterSoundMap.Contains(MonsterType))
	{
		if (SoundBuffer.MonsterSoundMap[MonsterType].SoundMap.Contains(SoundType))
		{
			return true;
		}
	}

	if (!CheckValidOfMonsterSoundData())
	{
		return false;
	}

	USoundBase* SoundSource = nullptr;

	for (const FMonsterSoundDataRow& SoundData : SoundBuffer.MonsterData)
	{
		if (SoundData.MonsterType == MonsterType)
		{
			if (SoundData.SoundType == SoundType)
			{
				SoundSource = SoundData.SoundSource.LoadSynchronous();
				break;
			}
		}
	}

	if (!IsValid(SoundSource))
	{
		return false;
	}

	FSoundSourceData TempData;
	TempData.SoundMap.Add(SoundType, SoundSource);
	SoundBuffer.MonsterSoundMap.Add(MonsterType, TempData);

	return true;
}

bool USoundHandle::CheckValidOfMonsterSoundData()
{
	if (SoundBuffer.MonsterData.IsEmpty())
	{
		if (!IsValid(SoundSettings))
		{
			return false;
		}

		if (SoundSettings->MonsterSoundDataTable.IsNull())
		{
			return false;
		}

		TArray<FMonsterSoundDataRow*> TempArray;
		const FString DataContext(TEXT("Monster Data Load"));

		MonsterDataTable = SoundSettings->MonsterSoundDataTable.LoadSynchronous();
		MonsterDataTable->GetAllRows(DataContext, TempArray);

		for (const FMonsterSoundDataRow* MonsterSoundDataRow : TempArray)
		{
			SoundBuffer.MonsterData.Add(*MonsterSoundDataRow);
		}
	}

	return true;
}

bool USoundHandle::CheckValidOfNpcSoundSource(ENpcType NpcType, ESoundType SoundType)
{
	if (SoundBuffer.NpcSoundMap.Contains(NpcType))
	{
		if (SoundBuffer.NpcSoundMap[NpcType].SoundMap.Contains(SoundType))
		{
			return true;
		}
	}

	if (!CheckValidOfNpcSoundData())
	{
		return false;
	}

	USoundBase* SoundSource = nullptr;

	for (const FNpcSoundDataRow& SoundData : SoundBuffer.NpcData)
	{
		if (SoundData.NpcType == NpcType)
		{
			if (SoundData.SoundType == SoundType)
			{
				SoundSource = SoundData.SoundSource.LoadSynchronous();
				break;
			}
		}
	}

	if (!IsValid(SoundSource))
	{
		return false;
	}

	FSoundSourceData TempData;
	TempData.SoundMap.Add(SoundType, SoundSource);
	SoundBuffer.NpcSoundMap.Add(NpcType, TempData);

	return true;
}

bool USoundHandle::CheckValidOfNpcSoundData()
{
	if (SoundBuffer.NpcData.IsEmpty())
	{
		if (!IsValid(SoundSettings))
		{
			return false;
		}

		if (SoundSettings->NpcSoundDataTable.IsNull())
		{
			return false;
		}

		TArray<FNpcSoundDataRow*> TempArray;
		const FString DataContext(TEXT("Npc Data Load"));

		NpcDataTable = SoundSettings->NpcSoundDataTable.LoadSynchronous();
		NpcDataTable->GetAllRows(DataContext, TempArray);

		for (const FNpcSoundDataRow* NpcSoundDataRow : TempArray)
		{
			SoundBuffer.NpcData.Add(*NpcSoundDataRow);
		}
	}

	return true;
}

bool USoundHandle::CheckValidOfItemSoundSource(EItemType ItemType, ESoundType SoundType)
{
	if (SoundBuffer.ItemSoundMap.Contains(ItemType))
	{
		if (SoundBuffer.ItemSoundMap[ItemType].SoundMap.Contains(SoundType))
		{
			return true;
		}
	}

	if (!CheckValidOfItemSoundData())
	{
		return false;
	}

	USoundBase* SoundSource = nullptr;

	for (const FItemSoundDataRow& SoundData : SoundBuffer.ItemData)
	{
		if (SoundData.ItemType == ItemType)
		{
			if (SoundData.SoundType == SoundType)
			{
				SoundSource = SoundData.SoundSource.LoadSynchronous();
				break;
			}
		}
	}

	if (!IsValid(SoundSource))
	{
		return false;
	}

	FSoundSourceData TempData;
	TempData.SoundMap.Add(SoundType, SoundSource);
	SoundBuffer.ItemSoundMap.Add(ItemType, TempData);

	return true;
}

bool USoundHandle::CheckValidOfItemSoundData()
{
	if (SoundBuffer.ItemData.IsEmpty())
	{
		if (!IsValid(SoundSettings))
		{
			return false;
		}

		if (SoundSettings->ItemSoundDataTable.IsNull())
		{
			return false;
		}

		TArray<FItemSoundDataRow*> TempArray;
		const FString DataContext(TEXT("Item Data Load"));

		ItemDataTable = SoundSettings->ItemSoundDataTable.LoadSynchronous();
		ItemDataTable->GetAllRows(DataContext, TempArray);

		for (const FItemSoundDataRow* ItemSoundDataRow : TempArray)
		{
			SoundBuffer.ItemData.Add(*ItemSoundDataRow);
		}
	}

	return true;
}