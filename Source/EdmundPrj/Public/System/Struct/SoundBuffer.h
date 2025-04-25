// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "SoundBuffer.generated.h"

struct FBgmDataRow;
struct FUISoundDataRow;
struct FPlayerSoundDataRow;
struct FMonsterSoundDataRow;
struct FNpcSoundDataRow;
struct FItemSoundDataRow;

USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FSoundDataBuffer
{
	GENERATED_BODY()
	
public:
	TArray<FBgmDataRow*> BgmData;
	TArray<FUISoundDataRow*> UIData;
	TArray<FPlayerSoundDataRow*> PlayerData;
	TArray<FMonsterSoundDataRow*> MonsterData;
	TArray<FNpcSoundDataRow*> NpcData;
	TArray<FItemSoundDataRow*> ItemData;

	TMap<ECharacterType, TMap<ESoundType, TObjectPtr<USoundBase>>> PlayerSoundMap;
	TMap<EMonsterType, TMap<ESoundType, TObjectPtr<USoundBase>>> MonsterSoundMap;
	TMap<ENpcType, TMap<ESoundType, TObjectPtr<USoundBase>>> NpcSoundMap;
	TMap<EItemType, TMap<ESoundType, TObjectPtr<USoundBase>>> ItemSoundMap;
};
