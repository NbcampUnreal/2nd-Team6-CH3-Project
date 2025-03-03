// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System/EnumSet.h"
#include "MissionHandle.generated.h"

class AEdmundGameMode;
class AEdmundGameState;
class ABaseMissionItem;
class AMissionItemBossSpawnPoint;
class ANPCMonster;
class UBehaviorTree;
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

	void RequestSwapBgm(EBGMSoundType Type);

	void RequestPrintStory(int32 Index);

	// Npc Controll
	void SpawnNpc(const FVector& SpawnPos);
	void SetNpcBondageMode(bool bIsBondage);
	void SetNpcBattleMode(bool bIsBattle);
	void SetNpcMoveMode(bool bIsMove);

	// Mission1
	void SetPrison(ABaseMissionItem* NewPrison);
	ABaseMissionItem* GetPrison() const;

	// Mission2
	void SetTargetPointLocation(const FVector& TargetPointPos);
	void TeleportPlayerToTargetPoint();
	void NotifyStartDefenceMode();
	void ApplyNpcEquip();

	// Defence Progress
	void UpdateDefenceState(bool bIsOn);
	void UpdateDefenceProgress(float Value);

	// Mission3
	void ApplyBossWeaken();
	void AddAlter(ABaseMissionItem* Alter);
	void LockToBossMonsterSkill(ABaseMissionItem* Alter);
	void AddDimensionPortalSet(ABaseMissionItem* DimentionPortal);
	void RemoveDimensionPortalSet(ABaseMissionItem* DimentionPortal);

	// Boss
	void NotifyStartedBossStage(AMissionItemBossSpawnPoint* NewBossHandle);
	bool GetWeakenBoss() const;
	EBossState GetLockedSkill() const;
	void RequestSpawnToSpawnerHandle();
	void ApplyNextPatternFromHalf();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnMissionItem(UClass* SpawnClass, const FVector& SpawnPos, const FMissionDataRow* MissionData);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Npc")
	TSubclassOf<ANPCMonster> NpcClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Npc")
	TObjectPtr<UBehaviorTree> NpcBT;

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
	TObjectPtr<AMissionItemBossSpawnPoint> BossHandle = nullptr;

	TObjectPtr<ANPCMonster> NpcPawn;

	FVector TargetPointLocation = FVector::ZeroVector;

	int32 MainMissionIndex = -1;
	int32 SpawnerCountFromBoss = 0;
	EBossState LockTarget = EBossState::Idle;
	bool bGetNpcEquip = false;
	bool bWeakenBoss = false;
	bool bIsDefence = false;

	FTimerHandle TestTimer;
};
