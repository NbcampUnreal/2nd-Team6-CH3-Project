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
	FName RowName = TEXT("DefaultName");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENpcType NpcType = ENpcType::Baldor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESoundType SoundType = ESoundType::Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> SoundSource = nullptr;
};
