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

UCLASS()
class EDMUNDPRJ_API UDataHandle : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//Initialize - LoadDataTable By DataHandleSettings / Initialize Arrays
	void InitDataHandle(UEdmundGameInstance* NewGameInstance);

	// Control Player Advance State Data
	const bool UpdateCurrentAdvance(const FName& TargetRow, const int32 UpdateValue);
	const TArray<FShopCatalogRow*>& GetCurrentAdvance() const;
	const FShopCatalogRow* GetCurrentAdvanceByRowName(const FName& TargetRow) const;

	// Control Player Money Data
	void AddMoney(const int32 AddValue);
	const int32 GetMoney() const;
	const bool Consume(const int32 Price);

	// Control Player Type Data
	void SetPlayerType(const ECharacterType CharacterType);
	const ECharacterType GetPlayerType() const;

	void UpdateClearMission(const int32 Index);
	const bool IsClearedMission(const int32 Index) const;

	void UpdateShowedIntro(const bool bShowed);
	const bool IsShowedIntro() const;

	const UDataTable* GetDataTable(const ETableType TableType) const;

private:
	void LoadDataTables(const UDataHandleSettings* DataSettings);

private:
	UPROPERTY()
	TObjectPtr<UDataTable> ShopCatalogDataTable = nullptr;

	UPROPERTY()
	TObjectPtr<UDataTable> PlayDataTable = nullptr;

	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;

	TArray<FShopCatalogRow*> CurrentAdvance;
	TArray<FPlayDataRow*> PlayData;

	ECharacterType PlayerType = ECharacterType::Gunner;
};
