// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "System/EnumSet.h"
#include "DataHandle.generated.h"

class UEdmundGameInstance;
class UDataHandleSettings;
struct FShopCatalogRow;
struct FPlayDataRow;
struct FPlayerSkillRow;
struct FCharacterDataRow;
struct FMissionDataRow;
struct FSpawnerDataRow;

UCLASS()
class EDMUNDPRJ_API UDataHandle : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//Initialize - LoadDataTable By DataHandleSettings / Initialize Arrays
	void InitDataHandle(UEdmundGameInstance* NewGameInstance);

	// Controll Player Advance State Data
	const bool UpdateCurrentAdvance(const FName& TargetRow, const int32 UpdateValue);
	const TArray<FShopCatalogRow*>& GetCurrentAdvance() const;
	const FShopCatalogRow* GetCurrentAdvanceByRowName(const FName& TargetRow) const;

	// Controll Player Money Data
	void AddMoney(const int32 AddValue);
	const int32 GetMoney() const;
	const bool Consume(const int32 Price);

	// Controll Player Type Data
	void SetPlayerType(const ECharacterType CharacterType);
	const ECharacterType GetPlayerType() const;

	//Controll Player Skill Data
	const TArray<FPlayerSkillRow*>& GetPlayerSkillData() const;

	// Controll Character Data
	const TArray<FCharacterDataRow*>& GetCharacterData() const;
	UClass* GetCharacterClass() const;

	// Controll Mission Data
	const TArray<FMissionDataRow*>& GetMissionDataBySceneType(const ESceneType SceneType);

	// Controll Spawner Data
	const TArray<FSpawnerDataRow*>& GetSpawnerDataBySceneType(const ESceneType SceneType);

	// Controll Play Data
	void UpdateClearMission(const ESceneType SceneType);
	const bool GetIsClearedMission(const int32 Index) const;
	void UpdateShowedIntro(const bool bShowed);
	const bool GetIsShowedIntro() const;

private:
	void LoadDataTables(const UDataHandleSettings* DataSettings);
	FShopCatalogRow* SelectRow(const FName& TargetRow) const;

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

	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;

	TArray<FShopCatalogRow*> CurrentAdvance;
	TArray<FPlayDataRow*> PlayData;
	TArray<FPlayerSkillRow*> PlayerSkillData;
	TArray<FCharacterDataRow*> CharacterData;
	TArray<FMissionDataRow*> MissionData;
	TArray<FSpawnerDataRow*> SpawnerData;

	TArray<FMissionDataRow*> CurrentMissionData;
	TArray<FSpawnerDataRow*> CurrentSpawnerData;
};
