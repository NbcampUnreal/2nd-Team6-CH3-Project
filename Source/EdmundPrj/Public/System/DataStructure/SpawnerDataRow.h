// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "SpawnerDataRow.generated.h"

class AMonsterSpawner;

USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FSpawnerDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName = TEXT("DefaultName");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESceneType InSceneType = ESceneType::Ending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMonsterSpawner> SpawnerClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> SpawnLocationSet = TArray<FVector>();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> SpawnTimeSet = TArray<float>();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SpawnCountSet = TArray<int32>();
};
