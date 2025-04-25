// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "System/EnumSet.h"
#include "System/Struct/SoundSourceData.h"
#include "EdmundGameState.generated.h"

class UEdmundGameInstance;
class AEdmundGameMode;
class USoundHandle;
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
	AEdmundGameState();

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	void SetMissionHandle(AMissionHandle* NewMissionHandle);
	void SetSpawnerHandle(ASpawnerHandle* NewSpawnerHandle);

	void InitMainLevelPlayerController();

	void SetEffectVolume(const float Volume);
	void InitSkillData(const TArray<FPlayerSkillRow*> PlayerSkillDataSet);

	void PlayPlayerSound(UAudioComponent* AudioComp, ESoundType SoundType);
	void PlayMonsterSound(UAudioComponent* AudioComp, EMonsterType MonsterType, ESoundType SoundType);
	void PlayNpcSound(UAudioComponent* AudioComp, ENpcType NpcType, ESoundType SoundType);
	void PlayItemSound(UAudioComponent* AudioComp, EItemType ItemType, ESoundType SoundType);

	void PlaySoundSource(UAudioComponent* AudioComp, USoundBase* SoundSource);

	const TArray<FShopCatalogRow*>& GetPlayerAdvancedData() const;

	void ChangeCursorMode(bool bIsValid);
	void ChangeInputMode(const FInputModeDataBase& InputMode);

	void AddCurrentLevelMoney(int32 Money);

	void PrintStoryText(const FText& TargetText);
	void OnEndedCurrentStory();
	void StopPrintStory();
	void SkipCurrentStory();

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

	void EndCurrentLevel(bool bIsClear);

	void RegisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer);
	void UnregisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer);

	void NotifyUpdateNotifyText(const FString& NotifyText);
	void NotifyUpdateMissionText(const FString& MissionText);
	void NotifyPlayerHp(const int32 MaxHp, const int32 CurrentHp);
	void NotifyPlayerOther(const int32 MaxValue, const int32 CurrentValue);
	void NotifyPlayerAmmo(const int32 MaxAmmo, const int32 CurrentAmmo);
	void NotifyPlayerExp(const int32 MaxExp, const int32 CurrentExp);
	void NotifyPlayerLevel(const int32 LevelValue);
	void NotifyOnStageProgress(const FString& ProgressText, bool bIsOn);
	void NotifyUpdateStageProgress(const float Value);
	void NotifyBossHp(const int32 MaxHp, const int32 CurrentHp);
	void NotifyOnOverlapedDefenceArea(const bool bIsOverlaped);
	void NotifyOnMissionInfo();
	void NotifyPrintText(const FString& TargetText);
	void NotifyIsGameClear(const bool bIsClear);

	APlayerController* GetPlayerController();
	void SetPlayerPawn(AActor* NewPawn);
	AActor* GetPlayerPawn();

private:
	virtual void Tick(float DeltaTime) override;

	void CalculateSkillList(); 

	void NotifyCreateRandomSkill() const;
	void NotifySelectCharacterType(ECharacterType CharacterType) const;
	void NotifyResultValue(int32 CurrnetMoney, int32 TotalMoney, int32 MissionMoney) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	float IntervalTime = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	float SlowTime = 0.2f;

private:
	UPROPERTY()
	TMap<ESoundType, USoundBase*> PlayerSoundSet;

	UPROPERTY()
	TMap<EMonsterType, FSoundSourceData> MonsterSoundSet;

	UPROPERTY()
	TMap<ENpcType, FSoundSourceData> NpcSoundSet;

	UPROPERTY()
	TMap<EItemType, FSoundSourceData> ItemSoundSet;

	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;
	TObjectPtr<USoundHandle> SoundHandle = nullptr;
	TObjectPtr<AEdmundGameMode> EdmundGameMode = nullptr;
	TObjectPtr<APlayerController> PlayerController = nullptr;
	TObjectPtr<AActor> PlayerPawn = nullptr;

	TObjectPtr<AMissionHandle> MissionHandle = nullptr;
	TObjectPtr<ASpawnerHandle> SpawnerHandle = nullptr;

	TArray<TScriptInterface<IGameStateObserver>> Observers;

	TArray<FPlayerSkillRow*> SkillDataSet;
	TArray<FPlayerSkillRow*> RandomSkillSet;
	TMap<FPlayerSkillRow*, int32> CurrentSkillMap;

	FTimerHandle TimerHandle;

	int32 CurrentLevelMoney = 0;
	int32 MissionClearMoney = 1000;
	float EffectVolume = 0;
	ECharacterType CurrentCharacterType = ECharacterType::Gunner;
	FText StoryText = FText::GetEmpty();
	FString CurrentText = "";
	float CurrentTime = 0;
	int32 StoryIndex = -1;
	int32 StoryLastIndex = -1;
};
