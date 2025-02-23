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
struct FShopCatalogRow;

UCLASS()
class EDMUNDPRJ_API UEdmundGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// Init GameInstance - Get Subsystems and Initialize
	virtual void Init() override;

	// GameState By Current Level Ready Complete. Bind Observers and Add To Viewport Widget
	void StartedGameState();
	void BindGameStateObserver() const;
	void OnUIByScene() const;

	// Controll Game Tool
	void OnPause() const;
	void ChangeCursorMode(const bool bIsVisible) const;
	void ChangeInputMode(const FInputModeDataBase& InputMode) const;
	void ChangeSelectMode(const bool bIsSelect) const;
	void QuitGame() const;

	// Current Mission Cleared or Fail 
	void EndMission() const;
	void DestroyedGameState();

	// Scene Move Controll
	void RequestSceneMove(const bool bIsNext, ESceneType SceneType = ESceneType::Title) const;
	void MoveScene(const ESceneType SceneType) const;
	void MoveNextScene() const;
	ESceneType GetCurrentSceneName() const;

	// Game Data Controll
	const UDataTable* GetDataTable(const ETableType TableName) const;
	const TArray<FShopCatalogRow*>& GetAdvanceState() const;
	const FShopCatalogRow* GetAdvanceState(const FName& TargetRow) const;
	bool UpdateAdvanceState(const FName RowName, const int32 UpdateValue) const;
	ECharacterType GetPlayerType() const;
	void SetPlayerType(const ECharacterType Type) const;
	//FMissionItemRow* GetCurrentMissionInfo() const;
	void AddPossessMoney(const int32 Value) const;
	int32 GetPossessMoney() const;
	bool CheckClearedMission(const int32 Index) const;

	// player skill apply
	void OnSkillListUI() const;
	void ApplySelectSkill(const int32 Index) const;

	// Game Sound Controll
	void SetBGMVolume(const float Volume) const;
	float GetBGMVolume() const;
	void SetEffectVolume(const float Volume) const;
	float GetEffectVolume() const;
	void PlayBGMByScene() const;
	void PlayUISound(const int32 Index) const;
	void PlayEffectSound(const UAudioComponent* AudioComp, const ESoundType SoundCategory, const int32 Index) const;

protected:
	

private:

	
public:


protected:
	TObjectPtr<USceneHandle> SceneHandle;
	TObjectPtr<UUIHandle> UIHandle;
	TObjectPtr<USoundHandle> SoundHandle;
	TObjectPtr<UDataHandle> DataHandle;
	TObjectPtr<AEdmundGameState> EdmundGameState;
};
