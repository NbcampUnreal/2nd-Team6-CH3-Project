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
	void ApplyMissionDataInLevel();

	void OnBeginOverlapedItem(ABaseMissionItem* MissionItem);
	void OnEndOverlapedItem();
	void OnPressedKeyFromPlayer();

	void RequestUpdateMissionText(const FString& MissionText);
	void RequestUpdateNotifyText(const FString& NotifyText);

	void StartMainMission();
	void CompleteMission();
	// Mission1
	const FVector GetDirectionToPrison(const FVector& ActorPos) const;
	void SetPrisonLocation(const FVector& PrisonPos);

	// Mission2

	// Mission3
	void DecressSpawnerCountFromBoss();
	void LockToBossMonsterSkill(int32 SkillIndex);

protected:
	virtual void BeginPlay() override;

private:
	void SpawnMissionItem(UClass* SpawnClass, const FVector& SpawnPos, const FName& MissionType, const FString& MissionInfo);

private:
	TArray<FMissionDataRow*> MissionDataSet;
	TArray<ABaseMissionItem*> MissionItemSet;
	TArray<ABaseMissionItem*> MainMissionSet;

	TObjectPtr<AEdmundGameMode> EdmundGameMode = nullptr;
	TObjectPtr<AEdmundGameState> EdmundGameState = nullptr;

	TObjectPtr<ABaseMissionItem> TargetMissionItem = nullptr;

	FVector PrisonLocation = FVector::ZeroVector;

	int32 MainMissionIndex = 0;
	int32 SpawnerCountFromBoss = 0;

	FTimerHandle TestTimer;
};
