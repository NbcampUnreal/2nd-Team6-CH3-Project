// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemTeleportMachine.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

void AMissionItemTeleportMachine::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(true);
}

void AMissionItemTeleportMachine::ActionEventByPressedKey()
{
	if (bIsPlayingInteraction)
	{
		return;
	}

	Super::ActionEventByPressedKey();

	ActiveTeleportMachine();
}

void AMissionItemTeleportMachine::ActionBeginOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionBeginOverlap();

	InteractionWidget->VisibleNotify(true);
	PrintMissionActiveText();
}

void AMissionItemTeleportMachine::ActionEndOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionEndOverlap();

	InteractionWidget->VisibleNotify(false);
}

void AMissionItemTeleportMachine::ActiveTeleportMachine()
{
	PrintMissionClearText();
	MissionHandle->TeleportPlayerToTargetPoint();
}

