// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "PlayerSkillRow.generated.h"


USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FPlayerSkillRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EActiveSkillType ActiveSkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPassiveSkillType PassiveSkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETimerSkillType TimerSkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> SkillImage;
};
