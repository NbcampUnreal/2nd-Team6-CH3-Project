// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "System/EnumSet.h"
#include "MainLevelPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ABaseCharacter;
class AEdmundGameState;
struct FCharacterDataRow;

UCLASS()
class EDMUNDPRJ_API AMainLevelPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> MainLevelIMC = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LeftClickAction = nullptr;

	AMainLevelPlayerController();
	void SetTargetToNull();
	void SetSelectMode(bool Value);
	void InitMainLevelCharacters(const TArray<FCharacterDataRow*>& CharacterData, ECharacterType Type, AEdmundGameState* NewGameState);
	void CompareType(ECharacterType Type);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DelTime) override;
	virtual void SetupInputComponent() override;

private:
	void InputLeftMouseButtonClick();
	void CheckCollideCharacter();
	void MoveToTargetPoint();
	void MoveToStartPoint();
	
private:
	TObjectPtr<ABaseCharacter> TargetCharacter;
	TObjectPtr<AEdmundGameState> EdmundGameState;
	TArray <TObjectPtr<ABaseCharacter>> CharacterSet;

	TMap<TObjectPtr<ABaseCharacter>, FVector> DistanceByTarget;
	TMap<TObjectPtr<ABaseCharacter>, FVector> StartPosByCharacter;
	TMap<TObjectPtr<ABaseCharacter>, bool> bIsReturnByCharacter;
	FVector MoveTargetPos;
	FVector LookAtTargetPos;

	bool bIsSelectMode = true;
};
