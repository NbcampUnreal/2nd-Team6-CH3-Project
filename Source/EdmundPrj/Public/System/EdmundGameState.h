// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "System/EnumSet.h"
#include "EdmundGameState.generated.h"

class UEdmundGameInstance;
class AEdmundGameMode;

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

	void CalculateSkillList(); // 스킬 목록 계산, 변수로 가지고 있음. 계산한 목록은 Notify로 UI에 전달.
	void ApplySelectedSkill(const int32 Index); // 선택 완료 후 인덱스 넘어오면 랜덤 스킬 목록에서 인덱스로 추출 후 플레이어에 전달.

private:
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;
	TObjectPtr<AEdmundGameMode> EdmundGameMode = nullptr;
	TObjectPtr<APlayerController> PlayerController = nullptr;

	FTimerHandle TimerHandle;
};
