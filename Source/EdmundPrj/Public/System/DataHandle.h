// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "System/EnumSet.h"
#include "System/Struct/EdmundDataBuffer.h"
#include "DataHandle.generated.h"

class UEdmundGameInstance;
class UDataHandleSettings;
struct FShopCatalogRow;
struct FPlayDataRow;
struct FPlayerSkillRow;
struct FCharacterDataRow;
struct FMissionDataRow;
struct FSpawnerDataRow;
struct FStoryDataRow;

UCLASS()
class EDMUNDPRJ_API UDataHandle : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//Initialize - LoadDataTable By DataHandleSettings
	void InitDataHandle(UEdmundGameInstance* NewGameInstance);

	// Controll Player Advance State Data
	const bool UpdateCurrentAdvance(const FName& TargetRow, const int32 UpdateValue);
	const TArray<FShopCatalogRow*>& GetCurrentAdvance();
	const FShopCatalogRow* GetCurrentAdvanceByRowName(const FName& TargetRow);

	// Controll Player Money Data
	void AddMoney(const int32 AddValue);
	const int32 GetMoney();
	const bool Consume(const int32 Price);

	// Controll Player Type Data
	void SetPlayerType(const ECharacterType CharacterType);
	const ECharacterType GetPlayerType();

	//Controll Player Skill Data
	const TArray<FPlayerSkillRow*>& GetPlayerSkillData();

	// Controll Character Data
	const TArray<FCharacterDataRow*>& GetCharacterData();
	UClass* GetCharacterClass();

	// Controll Mission Data
	const TArray<FMissionDataRow*>& GetMissionDataBySceneType(const ESceneType SceneType);

	// Controll Spawner Data
	const TArray<FSpawnerDataRow*>& GetSpawnerDataBySceneType(const ESceneType SceneType);

	// Controll Story Data
	const TArray<FStoryDataRow*>& GetStoryDataBySceneType(const ESceneType SceneType);

	// Controll Play Data
	void UpdateClearMission(const ESceneType SceneType);
	const bool GetIsClearedMission(const int32 Index);
	void UpdateShowedIntro(const bool bShowed);
	const bool GetIsShowedIntro();

private:
	FShopCatalogRow* SelectAdvanceRow(const FName& TargetRow);

	bool CheckValidOfAdvanceData();
	bool CheckValidOfPlayData();
	bool CheckValidOfSkillData();
	bool CheckValidOfCharacterData();
	bool CheckValidOfMissionData();
	bool CheckValidOfSpawnData();
	bool CheckValidOfStoryData();

private:
	UPROPERTY()
	TObjectPtr<UDataTable> ShopCatalogDataTable = nullptr;

	UPROPERTY()
	TObjectPtr<UDataTable> PlayDataTable = nullptr;

	UPROPERTY()
	TObjectPtr<UDataTable> PlayerSkillDataTable = nullptr;

	UPROPERTY()
	TObjectPtr<UDataTable> CharacterDataTable = nullptr;

	UPROPERTY()
	TObjectPtr<UDataTable> MissionDataTable = nullptr;

	UPROPERTY()
	TObjectPtr<UDataTable> SpawnerDataTable = nullptr;

	UPROPERTY()
	TObjectPtr<UDataTable> StoryDataTable = nullptr;

	UPROPERTY()
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;

	UPROPERTY()
	FEdmundDataBuffer EdmundDataBuffer;

	UPROPERTY()
	const UDataHandleSettings* DataSettings = nullptr;
};
