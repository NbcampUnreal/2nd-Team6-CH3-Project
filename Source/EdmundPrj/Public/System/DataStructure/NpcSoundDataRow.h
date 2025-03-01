// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "NpcSoundDataRow.generated.h"

USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FNpcSoundDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENpcType NpcType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESoundType SoundType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> SoundSource;
};
