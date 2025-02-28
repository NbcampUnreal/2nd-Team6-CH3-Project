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
class ABaseCharacter;
struct FMissionDataRow;
struct FSpawnerDataRow;

UCLASS()
class EDMUNDPRJ_API AEdmundGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	//virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	void InitGameMode(UEdmundGameInstance* NewGameInstance, const TArray<FMissionDataRow*>& MissionDataSet, const TArray<FSpawnerDataRow*>& SpawnerDataSet);
	void StartMission(ESceneType CurrentScene);
	void ClearMission();
	void FailMission();

	void StartBossMission();
	void SpawnMonsterByBoss(const TArray<FVector>& ActiveDimensionPosSet);

	void SwapBgm(EBGMSoundType Type);

private:
	virtual void BeginPlay() override;
	void SpawnPlayerByCharacterType(ECharacterType Type);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	TSubclassOf<ABaseCharacter> GunnerClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	TSubclassOf<ABaseCharacter> AuroraClass = nullptr;

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
