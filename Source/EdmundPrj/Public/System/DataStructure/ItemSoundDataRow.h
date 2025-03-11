// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "ItemSoundDataRow.generated.h"

USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FItemSoundDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName = TEXT("DefaultName");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType = EItemType::AttackPlants;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESoundType SoundType = ESoundType::Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> SoundSource = nullptr;
};
