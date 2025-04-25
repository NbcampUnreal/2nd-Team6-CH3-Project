// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "SoundSourceData.generated.h"


USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FSoundSourceData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TMap<ESoundType, TObjectPtr<USoundBase>> SoundMap;

	//UPROPERTY()
	//ESoundType SoundType = ESoundType::Attack;

	//UPROPERTY()
	//TObjectPtr<USoundBase> SoundSource = nullptr;
};
