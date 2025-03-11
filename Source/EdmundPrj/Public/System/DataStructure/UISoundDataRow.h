// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "UISoundDataRow.generated.h"

USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FUISoundDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName = TEXT("DefaultName");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUISoundType Type = EUISoundType::Buy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> SoundSource = nullptr;
};
