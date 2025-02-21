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

UCLASS()
class EDMUNDPRJ_API UEdmundGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	void StartedGameState();
	void BindGameStateObserver() const;
	void QuitGame() const;

	void OnUIByScene() const;
	void OnPause() const;
	void ChangeCursorMode(const bool bIsVisible) const;
	void ChangeInputMode(const FInputModeDataBase& InputMode) const;
	void ChangeSelectMode(const bool bIsSelect) const;
	//void OnSkillList(const TArray<FPlayerSkillRow*>& SkillList);
	//void UpdatePlayerSkill(const FPlayerSkillRow* Skill);

	void EndMission() const;
	void DestroyedGameState();

	//void ReceiveSceneMove();
	void RequestSceneMove(const bool bIsNext, ESceneType SceneType = ESceneType::Title) const;
	void MoveScene(const ESceneType SceneType) const;
	void MoveNextScene() const;
	ESceneType GetCurrentSceneName() const;

	UDataTable* GetDataTable(const ETableType TableName) const;
	TMap<FName, int32>* GetPlayerInfo() const;
	void UpdatePlayerInfo(const FName Type, const int32 Price) const;
	ECharacterType GetPlayerType() const;
	void SetPlayerType(const ECharacterType Type) const;
	//FMissionItemRow* GetCurrentMissionInfo() const;
	void AddPossessMoney(const int32 Value) const;
	int32 GetPossessMoney() const;

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
