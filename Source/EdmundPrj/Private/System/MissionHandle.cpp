// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MissionHandle.h"
#include "System/Mission/BaseMissionItem.h"
#include "System/DataStructure/MissionDataRow.h"
#include "System/EdmundGameMode.h"
#include "System/EdmundGameState.h"

// Sets default values
AMissionHandle::AMissionHandle()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AMissionHandle::InitMissionHandle(const TArray<FMissionDataRow*>& MissionData, AEdmundGameMode* EdGameMode, AEdmundGameState* EdGameState)
{
	EdmundGameMode = EdGameMode;
	EdmundGameState = EdGameState;

	if (MissionData.Num() == 0)
	{
		return;
	}

	MissionDataSet = MissionData;

	//ApplyMissionDataInLevel();
}

const FVector AMissionHandle::GetDirectionToPrison(const FVector& ActorPos) const
{
	FVector Direction = PrisonLocation - ActorPos;
	Direction = Direction / Direction.Size();

	return Direction;
}

void AMissionHandle::SetPrisonLocation(const FVector& PrisonPos)
{
	PrisonLocation = PrisonPos;
}

void AMissionHandle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMissionHandle::ApplyMissionDataInLevel()
{
	for (const FMissionDataRow* MissionDataRow : MissionDataSet)
	{
		UClass* SpawnClass = MissionDataRow->MissionItemClass.Get();
		TArray<FVector> SpawnPosSet = MissionDataRow->SpawnLocationSet;

		for (const FVector& SpawnPos : SpawnPosSet)
		{
			SpawnMissionItem(SpawnClass, SpawnPos);
		}
	}

	// UI에 Mission Info 출력 구현 필요
}

void AMissionHandle::SpawnMissionItem(UClass* SpawnClass, const FVector& SpawnPos)
{
	FActorSpawnParameters SpawnParam;

	ABaseMissionItem* NewMissionItem = GetWorld()->SpawnActor<ABaseMissionItem>(SpawnClass, SpawnPos, FRotator::ZeroRotator, SpawnParam);

	MissionItems.Add(NewMissionItem);
	NewMissionItem->InitMissionItem(this);
}

