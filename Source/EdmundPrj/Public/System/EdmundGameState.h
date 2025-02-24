// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "System/EnumSet.h"
#include "EdmundGameState.generated.h"

class UEdmundGameInstance;
class AEdmundGameMode;
class IGameStateObserver;
struct FPlayerSkillRow;

UCLASS()
class EDMUNDPRJ_API AEdmundGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	void EndCurrentMission();

	void ChangeCursorMode(bool bIsValid);
	void ChangeInputMode(const FInputModeDataBase& InputMode);
	void OnClickedCharacter(const ECharacterType CharacterType);
	void ChangeSelectMode(const bool bIsSelect) const;

	void CreateRandomSkillSet();
	const TArray<FPlayerSkillRow*>& GetRandomSkillSet() const;
	void ApplySelectedSkill(const int32 Index); // 선택 완료 후 인덱스 넘어오면 랜덤 스킬 목록에서 인덱스로 추출 후 플레이어에 전달.

	void RegisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer);
	void UnregisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer);

private:
	void InitSkillData();
	void CalculateSkillList(); 

	void NotifyCreateRandomSkill() const;

private:
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;
	TObjectPtr<AEdmundGameMode> EdmundGameMode = nullptr;
	TObjectPtr<APlayerController> PlayerController = nullptr;

	TArray<TScriptInterface<IGameStateObserver>> Observers;

	TArray<FPlayerSkillRow*> SkillDataSet;
	TArray<FPlayerSkillRow*> RandomSkillSet;
	TMap<FPlayerSkillRow*, int32> CurrentSkillMap;

	FTimerHandle TimerHandle;
};
