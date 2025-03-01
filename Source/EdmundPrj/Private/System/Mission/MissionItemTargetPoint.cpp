// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemTargetPoint.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

void AMissionItemTargetPoint::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(false);

	MissionHandle->SetTargetPointLocation(GetActorLocation());
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
