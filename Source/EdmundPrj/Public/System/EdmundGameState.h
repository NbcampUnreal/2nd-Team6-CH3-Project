// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "System/EnumSet.h"
#include "EdmundGameState.generated.h"

class UEdmundGameInstance;
class AEdmundGameMode;
class IGameStateObserver;
class ABaseCharacter;
class AMissionHandle;
class ASpawnerHandle;
struct FPlayerSkillRow;
struct FShopCatalogRow;

UCLASS()
class EDMUNDPRJ_API AEdmundGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	void SetMissionHandle(AMissionHandle* NewMissionHandle);
	void SetSpawnerHandle(ASpawnerHandle* NewSpawnerHandle);

	void InitMainLevelPlayerController();

	void SetEffectVolume(const float Volume);

	void InitSoundMap(
		const TMap<ESoundType, TObjectPtr<USoundBase>> PlayerSound,
		const TMap<EMonsterType, TMap<ESoundType, TObjectPtr<USoundBase>>> MonsterSound,
		const TMap<ENpcType, TMap<ESoundType, TObjectPtr<USoundBase>>> NpcSound,
		const TMap<EItemType, TMap<ESoundType, TObjectPtr<USoundBase>>> ItemSound
	);
	void InitSkillData(const TArray<FPlayerSkillRow*> PlayerSkillDataSet);

	void PlayPlayerSound(UAudioComponent* AudioComp, ESoundType SoundType);
	void PlayMonsterSound(UAudioComponent* AudioComp, EMonsterType MonsterType, ESoundType SoundType);
	void PlayNpcSound(UAudioComponent* AudioComp, ENpcType NpcType, ESoundType SoundType);
	void PlayItemSound(UAudioComponent* AudioComp, EItemType ItemType, ESoundType SoundType);

	const TArray<FShopCatalogRow*>& GetPlayerAdvancedData() const;

	void ChangeCursorMode(bool bIsValid);
	void ChangeInputMode(const FInputModeDataBase& InputMode);

	void AddCurrentLevelMoney(int32 Money);

	void CreateRandomSkillSet();
	const TArray<FPlayerSkillRow*>& GetRandomSkillSet() const;
	void ApplySelectedSkill(const int32 Index);

	void SetSelectedCharacter(AActor* Character);
	void CancleSelectedCharacter();
	void CheckClosedPlayerType(ECharacterType Type);

	void OnPressedPauseKey();
	void RequestOnPause();
	void RequestEndPause();
	void RequestInteraction();

	void EndCurrentLevel();

	void RegisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer);
	void UnregisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer);

	void NotifyUpdateNotifyText(const FString& NotifyText);
	void NotifyUpdateMissionText(const FString& MissionText);
	void NotifyPlayerHp(const int32 MaxHp, const int32 CurrentHp);
	void NotifyPlayerOther(const int32 MaxValue, const int32 CurrentValue);
	void NotifyPlayerAmmo(const int32 MaxAmmo, const int32 CurrentAmmo);

	APlayerController* GetPlayerController();
	AActor* GetPlayerPawn();

private:
	void CalculateSkillList(); 

	void NotifyCreateRandomSkill() const;
	void NotifySelectCharacterType(ECharacterType CharacterType) const;

private:
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;
	TObjectPtr<AEdmundGameMode> EdmundGameMode = nullptr;
	TObjectPtr<APlayerController> PlayerController = nullptr;
	TObjectPtr<AActor> PlayerPawn = nullptr;

	TObjectPtr<AMissionHandle> MissionHandle;
	TObjectPtr<ASpawnerHandle> SpawnerHandle;

	TArray<TScriptInterface<IGameStateObserver>> Observers;

	TArray<FPlayerSkillRow*> SkillDataSet;
	TArray<FPlayerSkillRow*> RandomSkillSet;
	TMap<FPlayerSkillRow*, int32> CurrentSkillMap;

	TMap<ESoundType, TObjectPtr<USoundBase>> PlayerSoundSet;
	TMap<EMonsterType, TMap<ESoundType, TObjectPtr<USoundBase>>> MonsterSoundSet;
	TMap<ENpcType, TMap<ESoundType, TObjectPtr<USoundBase>>> NpcSoundSet;
	TMap<EItemType, TMap<ESoundType, TObjectPtr<USoundBase>>> ItemSoundSet;

	FTimerHandle TimerHandle;

	int32 CurrentLevelMoney = 0;
	float EffectVolume = 0;
	ECharacterType CurrentCharacterType = ECharacterType::Gunner;
};
