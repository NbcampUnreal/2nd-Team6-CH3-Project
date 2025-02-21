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
	FName AdvanceName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdvanceValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InfoString;
};
