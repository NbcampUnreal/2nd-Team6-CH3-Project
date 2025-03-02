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

void USoundHandle::InitSoundHandle(UEdmundGameInstance* NewGameInstance)
{
	EdmundGameInstance = NewGameInstance;

	const USoundHandleSettings* SoundSetting = GetDefault<USoundHandleSettings>();
	checkf(IsValid(SoundSetting), TEXT("SoundSetting is invalid"));

	LoadSoundData(SoundSetting);
	InitBgm();
	InitUISound();
	InitPlayerSoundMap();
	InitMonsterSoundMap();
	InitNpcSoundMap();
	InitItemSoundMap();
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
	USoundBase* SoundSource = BgmSet[BgmSoundType];

	if (!IsValid(SoundSource))
	{
		return;
	}

	if (!IsValid(BgmAudio))
	{
		BgmAudio = UGameplayStatics::CreateSound2D(EdmundGameInstance, SoundSource, BGMVolume); // 레벨 바뀔 때마다 없어서 생성해줌
		BgmAudio->bAutoDestroy = false;
	}

	if (BgmAudio->IsPlaying())
	{
		BgmAudio->Stop();
	}

	BgmAudio->SetSound(SoundSource);
	BgmAudio->Play();
}

void USoundHandle::PlayUISound(EUISoundType UISoundType)
{
	USoundBase* SoundSource = UISet[UISoundType];

	if (!IsValid(SoundSource))
	{
		return;
	}

	if (!IsValid(UIAudio))
	{
		UIAudio = UGameplayStatics::CreateSound2D(EdmundGameInstance, SoundSource, EffectVolume);
		UIAudio->bAutoDestroy = false;
	}

	UIAudio->SetSound(SoundSource);
	UIAudio->Play();
}

const TMap<ECharacterType, TMap<ESoundType, TObjectPtr<USoundBase>>> USoundHandle::GetPlayerSoundMap() const
{
	return PlayerSoundMap;
}

const TMap<EMonsterType, TMap<ESoundType, TObjectPtr<USoundBase>>> USoundHandle::GetMonsterSoundMap() const
{
	return MonsterSoundMap;
}

const TMap<ENpcType, TMap<ESoundType, TObjectPtr<USoundBase>>> USoundHandle::GetNpcSoundMap() const
{
	return NpcSoundMap;
}

const TMap<EItemType, TMap<ESoundType, TObjectPtr<USoundBase>>> USoundHandle::GetItemSoundMap() const
{
	return ItemSoundMap;
}

const TMap<ESoundType, TObjectPtr<USoundBase>>& USoundHandle::GetSoundMapByType(ESoundCategory Category, int32 TypeIndex) const
{
	switch (Category)
	{
	case ESoundCategory::Player:
		return PlayerSoundMap[(ECharacterType)TypeIndex];
		break;

	case ESoundCategory::Monster:
		return MonsterSoundMap[(EMonsterType)TypeIndex];
		break;

	case ESoundCategory::Npc:
		return NpcSoundMap[(ENpcType)TypeIndex];
		break;

	case ESoundCategory::Item:
		return ItemSoundMap[(EItemType)TypeIndex];
		break;

	default:
		checkNoEntry();
		break;
	}

	return ReturnFail;
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

void USoundHandle::LoadSoundData(const USoundHandleSettings* SoundSettings)
{
	BgmDataTable = SoundSettings->BgmSoundDataTable.LoadSynchronous();
	UIDataTable = SoundSettings->UISoundDataTable.LoadSynchronous();
	PlayerDataTable = SoundSettings->PlayerSoundDataTable.LoadSynchronous();
	MonsterDataTable = SoundSettings->MonsterSoundDataTable.LoadSynchronous();
	NpcDataTable = SoundSettings->NpcSoundDataTable.LoadSynchronous();
	ItemDataTable = SoundSettings->ItemSoundDataTable.LoadSynchronous();

	const FString ContextText(TEXT("Sound Data Context"));

	BgmDataTable->GetAllRows(ContextText, BgmData);
	UIDataTable->GetAllRows(ContextText, UIData);
	PlayerDataTable->GetAllRows(ContextText, PlayerData);
	MonsterDataTable->GetAllRows(ContextText, MonsterData);
	NpcDataTable->GetAllRows(ContextText, NpcData);
	ItemDataTable->GetAllRows(ContextText, ItemData);
}

void USoundHandle::InitBgm()
{
	for (const FBgmDataRow* BgmDataRow : BgmData)
	{
		BgmSet.Add({ BgmDataRow->Type, BgmDataRow->SoundSource.LoadSynchronous()});
	}
}

void USoundHandle::InitUISound()
{
	for (const FUISoundDataRow* UIDataRow : UIData)
	{
		UISet.Add({ UIDataRow->Type, UIDataRow->SoundSource.LoadSynchronous()});
	}
}

void USoundHandle::InitPlayerSoundMap()
{
	for (int32 TypeNum = (int32)ECharacterType::Gunner; TypeNum <= (int32)ECharacterType::Sparrow; TypeNum++)
	{
		ECharacterType CharacterType = (ECharacterType)TypeNum;
		TMap<ESoundType, TObjectPtr<USoundBase>> SoundSource;

		for (const FPlayerSoundDataRow* PlayerSoundDataRow : PlayerData)
		{
			if (PlayerSoundDataRow->CharacterType == CharacterType)
			{
				SoundSource.Add(PlayerSoundDataRow->SoundType, PlayerSoundDataRow->SoundSource.LoadSynchronous());
			}
		}

		PlayerSoundMap.Add(CharacterType, SoundSource);
	}
}

void USoundHandle::InitMonsterSoundMap()
{
	for (int32 TypeNum = (int32)EMonsterType::Melee; TypeNum <= (int32)EMonsterType::Boss; TypeNum++)
	{
		EMonsterType MonsterType = (EMonsterType)TypeNum;
		TMap<ESoundType, TObjectPtr<USoundBase>> SoundSource;

		for (const FMonsterSoundDataRow* MonsterSoundDataRow : MonsterData)
		{
			if (MonsterSoundDataRow->MonsterType == MonsterType)
			{
				SoundSource.Add(MonsterSoundDataRow->SoundType, MonsterSoundDataRow->SoundSource.LoadSynchronous());
			}
		}

		MonsterSoundMap.Add(MonsterType, SoundSource);
	}
}

void USoundHandle::InitNpcSoundMap()
{
	for (int32 TypeNum = (int32)ENpcType::Baldor; TypeNum <= (int32)ENpcType::Baldor; TypeNum++)
	{
		ENpcType NpcType = (ENpcType)TypeNum;
		TMap<ESoundType, TObjectPtr<USoundBase>> SoundSource;

		for (const FNpcSoundDataRow* NpcSoundDataRow : NpcData)
		{
			if (NpcSoundDataRow->NpcType == NpcType)
			{
				SoundSource.Add(NpcSoundDataRow->SoundType, NpcSoundDataRow->SoundSource.LoadSynchronous());
			}
		}

		NpcSoundMap.Add(NpcType, SoundSource);
	}
}

void USoundHandle::InitItemSoundMap()
{
	for (int32 TypeNum = (int32)EItemType::Gold; TypeNum <= (int32)EItemType::HealKit; TypeNum++)
	{
		EItemType ItemType = (EItemType)TypeNum;
		TMap<ESoundType, TObjectPtr<USoundBase>> SoundSource;

		for (const FItemSoundDataRow* ItemSoundDataRow : ItemData)
		{
			if (ItemSoundDataRow->ItemType == ItemType)
			{
				SoundSource.Add(ItemSoundDataRow->SoundType, ItemSoundDataRow->SoundSource.LoadSynchronous());
			}
		}

		ItemSoundMap.Add(ItemType, SoundSource);
	}
}