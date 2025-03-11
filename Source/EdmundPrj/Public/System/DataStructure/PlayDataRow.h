// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "PlayDataRow.generated.h"


USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FPlayDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterType CharacterType = ECharacterType::Aurora;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowedIntro = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bClearedMission1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bClearedMission2 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bClearedMission3 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PossessMoney = 0;
};
