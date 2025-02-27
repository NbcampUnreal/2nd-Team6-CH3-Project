// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemTeleportMachine.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

void AMissionItemTeleportMachine::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type, const FString& MissionInfo)
{
	Super::InitMissionItem(NewMissionHandle, Type, MissionInfo);

	ApplyOverlapCollision(true);
}

void AMissionItemTeleportMachine::ActionEventByPressedKey()
{
	if (bIsPlayingInteraction)
	{
		return;
	}

	Super::ActionEventByPressedKey();
}

void AMissionItemTeleportMachine::ActionBeginOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionBeginOverlap();
}

void AMissionItemTeleportMachine::ActionEndOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionEndOverlap();
}
