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
	void BindGameStateObserver();
	void QuitGame();

	void ActiveFade(bool bIsFadeIn);
	void OnUIByScene();
	void OnPause();
	void ChangeCursorMode(bool bIsVisible);
	void ChangeInputMode(const FInputModeDataBase& InputMode);
	//void OnSkillList(const TArray<FPlayerSkillRow*>& SkillList);
	//void UpdatePlayerSkill(const FPlayerSkillRow* Skill);

	void EndMission();
	void DestroyedGameState();

	void MoveScene(ESceneName SceneName);
	void MoveNextScene();
	ESceneName GetCurrentSceneName() const;

	UDataTable* GetDataTable(ETableName TableName) const;
	TMap<FName, int32>* GetPlayerInfo() const;
	void UpdatePlayerInfo(FName Type, int32 Price);
	ECharacterType GetPlayerType() const;
	void SetPlayerType(ECharacterType Type);
	//FMissionItemRow* GetCurrentMissionInfo() const;
	void AddPossessMoney(int32 Value);
	int32 GetPossessMoney() const;

	void SetBGMVolume(float Volume);
	void SetEffectVolume(float Volume);
	void PlayBGMByScene();
	void PlayUISound(int32 Index);
	void PlayEffectSound(UAudioComponent* AudioComp, ESoundCategory SoundCategory, int32 Index);
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
