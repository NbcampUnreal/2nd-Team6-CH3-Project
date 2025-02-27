// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "System/EnumSet.h"
#include "EdmundGameMode.generated.h"

class UEdmundGameInstance;
class AEdmundGameState;
class AMissionHandle;
class ASpawnerHandle;
struct FMissionDataRow;
struct FSpawnerDataRow;

UCLASS()
class EDMUNDPRJ_API AEdmundGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void InitGameMode(UEdmundGameInstance* NewGameInstance, const TArray<FMissionDataRow*>& MissionDataSet, const TArray<FSpawnerDataRow*>& SpawnerDataSet);
	void StartMission(ESceneType CurrentScene);
	void ClearMission();
	void FailMission();

private:
	virtual void BeginPlay() override;
	void InitDefaultPawnByCharacterType();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	TSubclassOf<AActor> GunnerClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	TSubclassOf<APlayerController> TestPlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	TSubclassOf<AActor> MissionHandleClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	TSubclassOf<AActor> SpawnerHandleClass = nullptr;

private:
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;
	TObjectPtr<AEdmundGameState> EdmundGameState = nullptr;
	TObjectPtr<AMissionHandle> MissionHandle = nullptr;
	TObjectPtr<ASpawnerHandle> SpawnerHandle = nullptr;
};
