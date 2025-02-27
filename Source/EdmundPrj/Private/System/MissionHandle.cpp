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
}

void AMissionHandle::OnBeginOverlapedItem(ABaseMissionItem* MissionItem)
{
	TargetMissionItem = MissionItem;

	//GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &ThisClass::OnPressedKeyFromPlayer, 2.0f, false);
}

void AMissionHandle::OnEndOverlapedItem()
{
	TargetMissionItem = nullptr;

	//GetWorld()->GetTimerManager().ClearTimer(TestTimer);
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
	MainMissionSet[MainMissionIndex]->PrintMissionInfoText();
	MainMissionSet[MainMissionIndex]->SetIsActive(true);
}

void AMissionHandle::CompleteMission()
{
	checkf(MainMissionIndex < MainMissionSet.Num(), TEXT("Main Mission Index out of range"));
	MainMissionSet[MainMissionIndex]->PrintMissionClearText();

	

	++MainMissionIndex;

	if (MainMissionIndex == MainMissionSet.Num())
	{
		EdmundGameMode->ClearMission();
	}
	else
	{
		StartMainMission();
	}
}

void AMissionHandle::SetPrison(ABaseMissionItem* NewPrison)
{
	Prison = NewPrison;
}

ABaseMissionItem* AMissionHandle::GetPrison() const
{
	if (!IsValid(Prison))
	{
		return nullptr;
	}
	return Prison;
}

void AMissionHandle::SetTargetPointLocation(const FVector& TargetPointPos)
{
	TargetPointLocation = TargetPointPos;
}

void AMissionHandle::TeleportPlayerToTargetPoint()
{
	AActor* PlayerPawn = EdmundGameState->GetPlayerPawn();

	FVector TargetVector = TargetPointLocation - FVector(-200, -200, 0);
	PlayerPawn->SetActorLocation(TargetVector);
}

void AMissionHandle::ApplyNpcEquip()
{
	bGetNpcEquip = true;
	// npc가 도움 주게 변경 필요
}

void AMissionHandle::ApplyBossWeaken()
{
	bWeakenBoss = true;
	// boss 초기화 시 확인 필요
}

void AMissionHandle::AddAlter(ABaseMissionItem* Alter)
{
	if (!IsValid(Alter))
	{
		return;
	}

	AlterSet.Add(Alter);
}

void AMissionHandle::LockToBossMonsterSkill(ABaseMissionItem* Alter)
{
	if (!IsValid(Alter))
	{
		return;
	}

	for (ABaseMissionItem* TargetAlter : AlterSet)
	{
		TargetAlter->SetIsActive(false);
	}

	int32 SkillIndex = AlterSet.Find(Alter);
	
	switch (SkillIndex)
	{
	case 0:
		LockTarget = EBossState::Attack1;
		break;

	case 1:
		LockTarget = EBossState::Attack2;
		break;

	case 2:
		LockTarget = EBossState::Attack3;
		break;

	case 3:
		LockTarget = EBossState::Attack4;
		break;

	default:
		LockTarget = EBossState::Idle;
		break;
	}
}

void AMissionHandle::AddDimensionPortalSet(ABaseMissionItem* DimentionPortal)
{
	if (!IsValid(DimentionPortal))
	{
		return;
	}

	DimensionPortalSet.Add(DimentionPortal);
}

void AMissionHandle::RemoveDimensionPortalSet(ABaseMissionItem* DimentionPortal)
{
	if (!IsValid(DimentionPortal))
	{
		return;
	}

	DimensionPortalSet.Remove(DimentionPortal);
}

void AMissionHandle::NotifyStartedBossStage()
{
	EdmundGameMode->StartBossMission();
}

bool AMissionHandle::GetWeakenBoss() const
{
	return bWeakenBoss;
}

EBossState AMissionHandle::GetLockedSkill() const
{
	return LockTarget;
}

void AMissionHandle::RequestSpawnToSpawnerHandle()
{
	TArray<FVector> DimensionPosSet;

	for (ABaseMissionItem* Dimension : DimensionPortalSet)
	{
		DimensionPosSet.Add(Dimension->GetActorLocation());
		Dimension->SetIsActive(false);
	}

	EdmundGameMode->SpawnMonsterByBoss(DimensionPosSet);
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
			SpawnMissionItem(SpawnClass, SpawnPos, MissionDataRow);
		}
	}

	MainMissionIndex = 0;
	StartMainMission();
}

void AMissionHandle::SpawnMissionItem(UClass* SpawnClass, const FVector& SpawnPos, const FMissionDataRow* MissionData)
{
	checkf(IsValid(SpawnClass), TEXT("Mission Item Class is invalid"));

	FActorSpawnParameters SpawnParam;

	ABaseMissionItem* NewMissionItem = GetWorld()->SpawnActor<ABaseMissionItem>(SpawnClass, SpawnPos, FRotator::ZeroRotator, SpawnParam);

	MissionItemSet.Add(NewMissionItem);

	FName MissionType = MissionData->MissionType;
	FString InfoText = MissionData->MissionInfoText;
	FString ActiveText = MissionData->MissionActiveText;
	FString ClearText = MissionData->MissionClearText;

	NewMissionItem->InitMissionItem(this, MissionType);
	NewMissionItem->SetMissionText(InfoText, ActiveText, ClearText);

	if (MissionType == "Main")
	{
		MainMissionSet.Add(NewMissionItem);
		NewMissionItem->SetIsActive(false);
	}
	else
	{
		NewMissionItem->SetIsActive(true);
	}
}

