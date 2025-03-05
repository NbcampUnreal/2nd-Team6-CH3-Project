// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemTargetPoint.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"
#include "Kismet/GameplayStatics.h"

void AMissionItemTargetPoint::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(false);
	
	FVector SpawnPos = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation() + FVector(0, 200, 0);
	MissionHandle->SetTargetPointLocation(GetActorLocation());
	MissionHandle->SpawnNpc(SpawnPos);
	MissionHandle->SetNpcMoveMode(true);
}

void AMissionItemTargetPoint::ActionEventByPressedKey()
{
	if (bIsPlayingInteraction)
	{
		return;
	}

	Super::ActionEventByPressedKey();

	bIsActive = false;
	SetVisible(false);
	MissionHandle->RequestSwapBgm(EBGMSoundType::Defence);
	MissionHandle->SetNpcBattleMode(true);
	MissionHandle->CompleteMission();
}

void AMissionItemTargetPoint::ActionBeginOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionBeginOverlap();

	InteractionWidget->VisibleNotify(true);
	PrintMissionActiveText();
}

void AMissionItemTargetPoint::ActionEndOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionEndOverlap();

	InteractionWidget->VisibleNotify(false);
}
