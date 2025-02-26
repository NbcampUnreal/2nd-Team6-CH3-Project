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

	ApplyMissionDataInLevel();
}

void AMissionHandle::OnBeginOverlapedItem(ABaseMissionItem* MissionItem)
{
	TargetMissionItem = MissionItem;

	GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &ThisClass::OnPressedKeyFromPlayer, 2.0f, false);
}

void AMissionHandle::OnEndOverlapedItem()
{
	TargetMissionItem = nullptr;

	GetWorld()->GetTimerManager().ClearTimer(TestTimer);
}

void AMissionHandle::OnPressedKeyFromPlayer()
{
	if (!IsValid(TargetMissionItem))
	{
		return;
	}
	TargetMissionItem->ActionEventByPressedKey();
}

void AMissionHandle::RequestUpdateMissionText(const FString& MissionText)
{
	checkf(IsValid(EdmundGameState), TEXT("GameState is invalid"));
	EdmundGameState->NotifyUpdateMissionText(MissionText);
}

void AMissionHandle::RequestUpdateNotifyText(const FString& NotifyText)
{
	checkf(IsValid(EdmundGameState), TEXT("GameState is invalid"));
	EdmundGameState->NotifyUpdateNotifyText(NotifyText);
}

void AMissionHandle::StartMainMission()
{
	checkf(MainMissionIndex < MainMissionSet.Num(), TEXT("Main Mission Index out of range"));
	MainMissionSet[MainMissionIndex]->PrintMissionText();
	MainMissionSet[MainMissionIndex]->SetIsActive(true);
}

void AMissionHandle::CompleteMission()
{
	++MainMissionIndex;

	if (MainMissionIndex == MainMissionSet.Num())
	{
		EdmundGameMode->EndMission();
	}
	else
	{
		StartMainMission();
	}
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
		FName MissionType = MissionDataRow->MissionType;
		FString MissionInfo = MissionDataRow->MissionInfoText;

		for (const FVector& SpawnPos : SpawnPosSet)
		{
			SpawnMissionItem(SpawnClass, SpawnPos, MissionType, MissionInfo);
		}
	}

	MainMissionIndex = 0;
	StartMainMission();
}

void AMissionHandle::SpawnMissionItem(UClass* SpawnClass, const FVector& SpawnPos, const FName& MissionType, const FString& MissionInfo)
{
	FActorSpawnParameters SpawnParam;

	ABaseMissionItem* NewMissionItem = GetWorld()->SpawnActor<ABaseMissionItem>(SpawnClass, SpawnPos, FRotator::ZeroRotator, SpawnParam);

	MissionItemSet.Add(NewMissionItem);
	NewMissionItem->InitMissionItem(this, MissionType, MissionInfo);
	NewMissionItem->SetIsActive(true);

	if (MissionType == "Main")
	{
		MainMissionSet.Add(NewMissionItem);
		NewMissionItem->SetIsActive(false);
	}
}

