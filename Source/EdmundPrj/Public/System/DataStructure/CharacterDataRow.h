// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/EnumSet.h"
#include "CharacterDataRow.generated.h"

class ABaseCharacter;

USTRUCT(BlueprintType)
struct EDMUNDPRJ_API FCharacterDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterType CharacterType = ECharacterType::Gunner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterName = TEXT("DefaultName");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterInfo = FText::FromString(TEXT("DefaultInfo"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnLocation = FVector::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseCharacter> CharacterClass = nullptr;
};
