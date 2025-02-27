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

	// Controll Game Tool
	void OnPause() const;
	void ChangeCursorMode(const bool bIsVisible) const;
	void ChangeInputMode(const FInputModeDataBase& InputMode) const;
	void QuitGame() const;

	// Current Mission Cleared or Fail 
	void EndMission(const bool bIsClear) const;
	bool CheckClearedMission(const int32 Index) const;
	void DestroyedGameState();

	// Scene Move Controll
	void RequestSceneMove(const bool bIsNext, ESceneType SceneType = ESceneType::Title) const;
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
	void PlayUISound(const int32 Index) const;
	void PlayEffectSound(const UAudioComponent* AudioComp, const ESoundType SoundCategory, const int32 Index) const;

private:
	// Level Data Controll
	const TArray<FMissionDataRow*>& GetCurrentMissionData(ESceneType SceneType) const;
	const TArray<FSpawnerDataRow*>& GetCurrentSpawnerData(ESceneType SceneType) const;
	
public:


private:
	TObjectPtr<USceneHandle> SceneHandle;
	TObjectPtr<UUIHandle> UIHandle;
	TObjectPtr<USoundHandle> SoundHandle;
	TObjectPtr<UDataHandle> DataHandle;
	TObjectPtr<AEdmundGameState> EdmundGameState;
	TObjectPtr<AEdmundGameMode> EdmundGameMode;
};
