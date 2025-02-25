// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionHandle.generated.h"

class AEdmundGameMode;
class AEdmundGameState;
class ABaseMissionItem;
struct FMissionDataRow;

UCLASS()
class EDMUNDPRJ_API AMissionHandle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMissionHandle();
	void InitMissionHandle(const TArray<FMissionDataRow*>& MissionData, AEdmundGameMode* EdGameMode, AEdmundGameState* EdGameState);

	// Mission1
	const FVector GetDirectionToPrison(const FVector& ActorPos) const;
	void SetPrisonLocation(const FVector& PrisonPos);

protected:
	virtual void BeginPlay() override;

private:
	void ApplyMissionDataInLevel();
	void SpawnMissionItem(UClass* SpawnClass, const FVector& SpawnPos);

private:
	TArray<FMissionDataRow*> MissionDataSet;
	TArray<ABaseMissionItem*> MissionItems;

	TObjectPtr<AEdmundGameMode> EdmundGameMode = nullptr;
	TObjectPtr<AEdmundGameState> EdmundGameState = nullptr;

	FVector PrisonLocation = FVector::ZeroVector;
};
