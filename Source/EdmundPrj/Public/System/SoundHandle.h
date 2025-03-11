// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "System/EnumSet.h"
#include "SoundHandle.generated.h"

class UEdmundGameInstance;
class USoundHandleSettings;
struct FBgmDataRow;
struct FUISoundDataRow;
struct FPlayerSoundDataRow;
struct FMonsterSoundDataRow;
struct FNpcSoundDataRow;
struct FItemSoundDataRow;

UCLASS()
class EDMUNDPRJ_API USoundHandle : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void InitSoundHandle(UEdmundGameInstance* NewGameInstance);

	// Play Bgm or UI sound
	void PlayBgmBySceneType(ESceneType SceneType);
	void PlayBgmByBgmType(EBGMSoundType BgmSoundType);
	void PlayUISound(EUISoundType UISoundType);

	// Get Sound Map
	const TMap<ESoundType, TObjectPtr<USoundBase>>& GetSoundMapByType(ESoundCategory Category, int32 TypeIndex) const;
	const TMap<ECharacterType, TMap<ESoundType, TObjectPtr<USoundBase>>> GetPlayerSoundMap() const;
	const TMap<EMonsterType, TMap<ESoundType, TObjectPtr<USoundBase>>> GetMonsterSoundMap() const;
	const TMap<ENpcType, TMap<ESoundType, TObjectPtr<USoundBase>>> GetNpcSoundMap() const;
	const TMap<EItemType, TMap<ESoundType, TObjectPtr<USoundBase>>> GetItemSoundMap() const;

	// Update Sound Volume By UI Sound Slider
	void UpdateBGMVolume(const float VolumeValue);
	void UpdateEffectVolume(const float VolumeValue);

	// Sound Volume Getter
	float GetBGMVolume() const;
	float GetEffectVolume() const;

private:
	void LoadSoundData(const USoundHandleSettings* SoundSettings);
	void InitBgm();
	void InitUISound();
	void InitPlayerSoundMap();
	void InitMonsterSoundMap();
	void InitNpcSoundMap();
	void InitItemSoundMap();

private:
	UPROPERTY()
	TObjectPtr<UDataTable> BgmDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> UIDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> PlayerDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> MonsterDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> NpcDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BgmAudio;

	UPROPERTY()
	TObjectPtr<UAudioComponent> UIAudio;

	TArray<FBgmDataRow*> BgmData;
	TArray<FUISoundDataRow*> UIData;
	TArray<FPlayerSoundDataRow*> PlayerData;
	TArray<FMonsterSoundDataRow*> MonsterData;
	TArray<FNpcSoundDataRow*> NpcData;
	TArray<FItemSoundDataRow*> ItemData;

	UPROPERTY()
	TMap<EBGMSoundType, TObjectPtr<USoundBase>> BgmSet;
	UPROPERTY()
	TMap<EUISoundType, TObjectPtr<USoundBase>> UISet;
	UPROPERTY()
	TMap<ESoundType, TObjectPtr<USoundBase>> ReturnFail;

	TMap<ECharacterType, TMap<ESoundType, TObjectPtr<USoundBase>>> PlayerSoundMap;
	TMap<EMonsterType, TMap<ESoundType, TObjectPtr<USoundBase>>> MonsterSoundMap;
	TMap<ENpcType, TMap<ESoundType, TObjectPtr<USoundBase>>> NpcSoundMap;
	TMap<EItemType, TMap<ESoundType, TObjectPtr<USoundBase>>> ItemSoundMap;
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;
	float BGMVolume = 0.8f;
	float EffectVolume = 0.8f;
};
