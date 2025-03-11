// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "StoryDataRow.generated.h"


USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FStoryDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESceneType SceneType = ESceneType::Ending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> StoryText = TArray<FText>();
};
