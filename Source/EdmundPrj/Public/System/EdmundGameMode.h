// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "System/EnumSet.h"
#include "EdmundGameMode.generated.h"

class UEdmundGameInstance;
class AEdmundGameState;
class AMissionHandle;

UCLASS()
class EDMUNDPRJ_API AEdmundGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	

private:
	virtual void BeginPlay() override;
	void InitDefaultPawnByCharacterType();
	void InitMission();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	TSubclassOf<AActor> GunnerClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	TSubclassOf<APlayerController> TestPlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	TSubclassOf<AActor> MissionHandleClass = nullptr;

private:
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;
	TObjectPtr<AEdmundGameState> EdmundGameState = nullptr;
	TObjectPtr<AMissionHandle> MissionHandle = nullptr;

	ESceneType CurrentScene = ESceneType::Title;
};
