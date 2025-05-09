// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "System/EnumSet.h"
#include "EdmundGameInstance.generated.h"

class USceneHandle;
class UUIHandle;
class USoundHandle;
class UDataHandle;
class AEdmundGameState;
class AEdmundGameMode;
class IGameStateObserver;
struct FShopCatalogRow;
struct FPlayerSkillRow;
struct FCharacterDataRow;
struct FMissionDataRow;
struct FSpawnerDataRow;

UCLASS()
class EDMUNDPRJ_API UEdmundGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// Init GameInstance - Get Subsystems and Initialize
	virtual void Init() override;

	// GameState By Current Level Ready Complete. Bind Observers and Add To Viewport Widget
	void RequestGameStart(AEdmundGameMode* NewGameMode, AEdmundGameState* NewGameState);
	void ApplyCurrentDataToGameMode();
	void StartMission(ESceneType SceneType);
	void BindGameStateObserver() const;
	void OnUIByScene() const;

	void BindUIObserverToGameState(const TScriptInterface<IGameStateObserver> Target);

	// Controll Game Tool
	void OnPause() const;
	void RequestPause() const;
	void RequestUnpause() const;
	void ChangeCursorMode(const bool bIsVisible) const;
	void ChangeInputMode(const FInputModeDataBase& InputMode) const;
	void QuitGame() const;

	// Current Mission Cleared or Fail 
	void EndMission(const bool bIsClear) const;
	bool CheckClearedMission(const int32 Index) const;
	void DestroyedGameState();

	// Game Story Controll
	void VisibleMissionStory() const;
	void InvisibleMissionStory() const;
	void MoveNextMissionStory() const;
	void SkipMissionStory() const;

	// Scene Move Controll
	void MoveScene(const ESceneType SceneType) const;
	void MoveNextScene() const;
	ESceneType GetCurrentSceneName() const;

	// Shop and Player Advance State Controll
	const TArray<FShopCatalogRow*>& GetAdvanceState() const;
	const FShopCatalogRow* GetAdvanceState(const FName& TargetRow) const;
	bool UpdateAdvanceState(const FName RowName, const int32 UpdateValue) const;

	// Player Type Controll
	ECharacterType GetPlayerType() const;
	void SetPlayerType(const ECharacterType Type) const;
	void CancleSelectedType();
	void CheckClosedPlayerType();
	const TArray<FCharacterDataRow*>& GetCharacterData() const;

	// Player Money Controll
	void AddPossessMoney(const int32 Value) const;
	int32 GetPossessMoney() const;

	// player skill apply
	void OnSkillListUI() const;
	void ApplySelectSkill(const int32 Index) const;
	const TArray<FPlayerSkillRow*>& GetPlayerSkillData() const;
	const TArray<FPlayerSkillRow*>& GetRandomSkillSet() const;

	// Game Sound Controll
	void SetBGMVolume(const float Volume) const;
	float GetBGMVolume() const;
	void SetEffectVolume(const float Volume) const;
	float GetEffectVolume() const;
	void PlayBGMByScene() const;
	void PlayBGM(const EBGMSoundType Type) const;
	void PlayUISound(const EUISoundType Type) const;

private:
	// Level Data Controll
	const TArray<FMissionDataRow*>& GetCurrentMissionData(ESceneType SceneType) const;
	const TArray<FSpawnerDataRow*>& GetCurrentSpawnerData(ESceneType SceneType) const;
	
public:


private:
	UPROPERTY()
	TObjectPtr<USceneHandle> SceneHandle = nullptr;

	UPROPERTY()
	TObjectPtr<UUIHandle> UIHandle = nullptr;

	UPROPERTY()
	TObjectPtr<USoundHandle> SoundHandle = nullptr;

	UPROPERTY()
	TObjectPtr<UDataHandle> DataHandle = nullptr;

	UPROPERTY()
	TObjectPtr<AEdmundGameState> EdmundGameState = nullptr;

	UPROPERTY()
	TObjectPtr<AEdmundGameMode> EdmundGameMode = nullptr;
};
