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

	void CreateRandomSkillSet();
	const TArray<FPlayerSkillRow*>& GetRandomSkillSet() const;
	void ApplySelectedSkill(const int32 Index);

	void SetSelectedCharacter(AActor* Character);
	void CancleSelectedCharacter();

	void RegisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer);
	void UnregisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer);

private:
	void InitMainLevel();
	void InitSkillData();
	void CalculateSkillList(); 

	void NotifyCreateRandomSkill() const;
	void NotifySelectCharacterType(ECharacterType CharacterType) const;

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
