// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System/EnumSet.h"
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
	void SetPrison(ABaseMissionItem* NewPrison);
	ABaseMissionItem* GetPrison() const;

	// Mission2
	void SetTargetPointLocation(const FVector& TargetPointPos);
	void TeleportPlayerToTargetPoint();
	void ApplyNpcEquip();

	// Mission3
	void ApplyBossWeaken();
	void AddAlter(ABaseMissionItem* Alter);
	void LockToBossMonsterSkill(ABaseMissionItem* Alter);
	void AddDimensionPortalSet(ABaseMissionItem* DimentionPortal);
	void RemoveDimensionPortalSet(ABaseMissionItem* DimentionPortal);

	// Boss
	void NotifyStartedBossStage();
	bool GetWeakenBoss() const;
	EBossState GetLockedSkill() const;
	void RequestSpawnToSpawnerHandle();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnMissionItem(UClass* SpawnClass, const FVector& SpawnPos, const FMissionDataRow* MissionData);

private:
	TArray<FMissionDataRow*> MissionDataSet;
	TArray<ABaseMissionItem*> MissionItemSet;
	TArray<ABaseMissionItem*> MainMissionSet;
	TArray<ABaseMissionItem*> AlterSet;
	TArray<ABaseMissionItem*> DimensionPortalSet;

	TObjectPtr<AEdmundGameMode> EdmundGameMode = nullptr;
	TObjectPtr<AEdmundGameState> EdmundGameState = nullptr;

	TObjectPtr<ABaseMissionItem> TargetMissionItem = nullptr;
	TObjectPtr<ABaseMissionItem> Prison = nullptr;

	FVector TargetPointLocation = FVector::ZeroVector;

	int32 MainMissionIndex = 0;
	int32 SpawnerCountFromBoss = 0;
	EBossState LockTarget = EBossState::Idle;
	bool bGetNpcEquip = false;
	bool bWeakenBoss = false;

	FTimerHandle TestTimer;
};
