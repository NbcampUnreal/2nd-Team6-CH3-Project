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
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESceneType InSceneType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseMissionItem> MissionItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> SpawnLocationSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MissionInfoText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MissionClearText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MissionActiveText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MissionType;
	
};
