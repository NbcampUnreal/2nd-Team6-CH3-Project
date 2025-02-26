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
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESceneType InSceneType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMonsterSpawner> SpawnerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> SpawnLocationSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> SpawnTimeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SpawnCountSet;
};
