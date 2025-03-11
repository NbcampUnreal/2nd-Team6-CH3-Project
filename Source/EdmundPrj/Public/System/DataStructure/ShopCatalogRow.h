// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ShopCatalogRow.generated.h"


USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FShopCatalogRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AdvanceName = TEXT("DefaultName");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OriginPrice = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdvanceValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MultiplyValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentPrice = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> SkillImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InfoString = TEXT("DefaultInfo");
};
