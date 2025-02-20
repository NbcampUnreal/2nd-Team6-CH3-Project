// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataHandle.generated.h"

class UEdmundGameInstance;
class UDataHandleSettings;

UCLASS()
class EDMUNDPRJ_API UDataHandle : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void InitDataHandle(UEdmundGameInstance* NewGameInstance);

private:
	void LoadDataTables(const UDataHandleSettings* DataSettings);

private:
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance;
	TObjectPtr<UDataTable> ShopCatalogDataTable;
};
