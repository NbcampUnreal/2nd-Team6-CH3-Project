// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumMonsterState.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),
	Patrol   UMETA(DisplayName = "Patrol"),
	Trace   UMETA(DisplayName = "Trace"),
	Attack       UMETA(DisplayName = "Attack"),
	Dead       UMETA(DisplayName = "Dead")
};

UCLASS()
class EDMUNDPRJ_API AEnumMonsterState : public AActor
{
	GENERATED_BODY()
};
