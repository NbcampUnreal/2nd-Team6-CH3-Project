// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "PlayerSoundDataRow.generated.h"

USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FPlayerSoundDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName = "DefaultName";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterType CharacterType = ECharacterType::Aurora;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESoundType SoundType = ESoundType::Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> SoundSource = nullptr;
};
