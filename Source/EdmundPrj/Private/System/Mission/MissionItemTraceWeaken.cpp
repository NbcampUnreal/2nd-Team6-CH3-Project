// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemTraceWeaken.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

void AMissionItemTraceWeaken::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(true);
}

void AMissionItemTraceWeaken::ActionEventByPressedKey()
{
	if (bIsPlayingInteraction)
	{
		return;
	}

	Super::ActionEventByPressedKey();

	FindTraceByPlayer();
}

void AMissionItemTraceWeaken::ActionBeginOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionBeginOverlap();

	PrintMissionActiveText();
	InteractionWidget->VisibleNotify(true);
}

void AMissionItemTraceWeaken::ActionEndOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionEndOverlap();

	InteractionWidget->VisibleNotify(false);
}

void AMissionItemTraceWeaken::FindTraceByPlayer()
{
	PrintMissionClearText();
	SetIsActive(false);
	SetVisible(false);
}
