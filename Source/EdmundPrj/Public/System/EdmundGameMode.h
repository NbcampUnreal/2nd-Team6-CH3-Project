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
struct FStoryDataRow;

UCLASS()
class EDMUNDPRJ_API AEdmundGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void InitGameMode(
		UEdmundGameInstance* NewGameInstance,
		const TArray<FMissionDataRow*>& MissionDataSet,
		const TArray<FSpawnerDataRow*>& SpawnerDataSet,
		const TArray<FStoryDataRow*>& StoryDataSet,
		UClass* CharacterClass
	);
	void StartMission(ESceneType CurrentScene);
	void ClearMission();
	void FailMission();

	void NotifyAllClearedMission();

	void OnStartedPrintStory(const int32 Index);
	void PrintCurrentStory();
	bool CheckRemainCurrentStory();
	void OnEndedCurrentStory();

	void StartDefenceMode();
	void StartBossMission();
	void SpawnMonsterByBoss(const TArray<FVector>& ActiveDimensionPosSet);
	void NotifyClearedHalfPatternByBoss();

	void SwapBgm(EBGMSoundType Type);

private:
	virtual void BeginPlay() override;
	void SpawnPlayerByCharacterType(UClass* SpawnClass);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	TSubclassOf<AActor> MissionHandleClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	TSubclassOf<AActor> SpawnerHandleClass = nullptr;

private:
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;
	TObjectPtr<AEdmundGameState> EdmundGameState = nullptr;
	TObjectPtr<AMissionHandle> MissionHandle = nullptr;
	TObjectPtr<ASpawnerHandle> SpawnerHandle = nullptr;

	TArray<FStoryDataRow*> MissionStoryData;
	FStoryDataRow* CurrentStoryData;

	int32 CurrentStoryIndex = 0;

	bool bIsCleared = false;
	bool bIsPlayingStory = false;
};
