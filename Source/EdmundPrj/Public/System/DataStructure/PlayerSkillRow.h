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
	FName RowName = TEXT("DefaultName");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillName = TEXT("DefaultName");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType = ESkillType::ActiveSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EActiveSkillType ActiveSkillType = EActiveSkillType::Null;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPassiveSkillType PassiveSkillType = EPassiveSkillType::Null;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETimerSkillType TimerSkillType = ETimerSkillType::Null;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillInfo = TEXT("DefulatInfo");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> SkillImage = nullptr;
};
