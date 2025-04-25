// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EdmundDataBuffer.generated.h"

struct FShopCatalogRow;
struct FPlayDataRow;
struct FPlayerSkillRow;
struct FCharacterDataRow;
struct FMissionDataRow;
struct FSpawnerDataRow;
struct FStoryDataRow;


USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FEdmundDataBuffer
{
	GENERATED_BODY()
	
public:
	TArray<FShopCatalogRow*> CurrentAdvance;
	TArray<FPlayDataRow*> PlayData;
	TArray<FPlayerSkillRow*> PlayerSkillData;
	TArray<FCharacterDataRow*> CharacterData;
	TArray<FMissionDataRow*> MissionData;
	TArray<FSpawnerDataRow*> SpawnerData;
	TArray<FStoryDataRow*> StoryData;

	TArray<FMissionDataRow*> CurrentMissionData;
	TArray<FSpawnerDataRow*> CurrentSpawnerData;
	TArray<FStoryDataRow*> CurrentStoryData;
};
