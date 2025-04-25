// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "System/Struct/SoundSourceData.h"
#include "System/DataStructure/BgmDataRow.h"
#include "System/DataStructure/UISoundDataRow.h"
#include "System/DataStructure/PlayerSoundDataRow.h"
#include "System/DataStructure/MonsterSoundDataRow.h"
#include "System/DataStructure/NpcSoundDataRow.h"
#include "System/DataStructure/ItemSoundDataRow.h"
#include "SoundBuffer.generated.h"


USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FSoundDataBuffer
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FBgmDataRow> BgmData;

	UPROPERTY()
	TArray<FUISoundDataRow> UIData;

	UPROPERTY()
	TArray<FPlayerSoundDataRow> PlayerData;

	UPROPERTY()
	TArray<FMonsterSoundDataRow> MonsterData;

	UPROPERTY()
	TArray<FNpcSoundDataRow> NpcData;

	UPROPERTY()
	TArray<FItemSoundDataRow> ItemData;

	UPROPERTY()
	TMap<ECharacterType, FSoundSourceData> PlayerSoundMap;

	UPROPERTY()
	TMap<EMonsterType, FSoundSourceData> MonsterSoundMap;

	UPROPERTY()
	TMap<ENpcType, FSoundSourceData> NpcSoundMap;

	UPROPERTY()
	TMap<EItemType, FSoundSourceData> ItemSoundMap;
};
