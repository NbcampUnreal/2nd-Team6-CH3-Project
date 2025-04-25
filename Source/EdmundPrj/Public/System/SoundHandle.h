// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "System/EnumSet.h"
#include "System/Struct/SoundBuffer.h"
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

	//Get Sound Source
	USoundBase* GetPlayerSound(ESoundType SoundType);
	USoundBase* GetMonsterSound(EMonsterType MonsterType, ESoundType SoundType);
	USoundBase* GetNpcSound(ENpcType NpcType, ESoundType SoundType);
	USoundBase* GetItemrSound(EItemType ItemType, ESoundType SoundType);

	// Update Sound Volume By UI Sound Slider
	void UpdateBGMVolume(const float VolumeValue);
	void UpdateEffectVolume(const float VolumeValue);

	// Sound Volume Getter
	float GetBGMVolume() const;
	float GetEffectVolume() const;

private:
	bool CheckValidOfBgmSource(EBGMSoundType Type);
	bool CheckValidOfBgmData();

	bool CheckValidOfUISoundSource(EUISoundType Type);
	bool CheckValidOfUISoundData();

	bool CheckValidOfPlayerSoundSource(ECharacterType CharacterType, ESoundType SoundType);
	bool CheckValidOfPlayerSoundData();

	bool CheckValidOfMonsterSoundSource(EMonsterType MonsterType, ESoundType SoundType);
	bool CheckValidOfMonsterSoundData();
	
	bool CheckValidOfNpcSoundSource(ENpcType NpcType, ESoundType SoundType);
	bool CheckValidOfNpcSoundData();
	
	bool CheckValidOfItemSoundSource(EItemType ItemType, ESoundType SoundType);
	bool CheckValidOfItemSoundData();
	
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

	UPROPERTY()
	TMap<EBGMSoundType, TObjectPtr<USoundBase>> BgmSet;

	UPROPERTY()
	TMap<EUISoundType, TObjectPtr<USoundBase>> UISet;

	UPROPERTY()
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;

	UPROPERTY()
	const USoundHandleSettings* SoundSettings = nullptr;

	UPROPERTY()
	FSoundDataBuffer SoundBuffer;

	UPROPERTY()
	float BGMVolume = 0.8f;

	UPROPERTY()
	float EffectVolume = 0.8f;
};
