// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "MissionDataRow.generated.h"

class ABaseMissionItem;

USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FMissionDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName = TEXT("DefaultName");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESceneType InSceneType = ESceneType::Main;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseMissionItem> MissionItemClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> SpawnLocationSet = TArray<FVector>();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MissionInfoText = TEXT("DefaultText");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MissionClearText = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MissionActiveText = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MissionType = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsContainStory = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBeginStory = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StoryIndex = 0;
};
