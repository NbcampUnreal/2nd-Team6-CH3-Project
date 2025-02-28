// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemWeakenBoss.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

void AMissionItemWeakenBoss::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(true);
}

void AMissionItemWeakenBoss::ActionEventByPressedKey()
{
	if (bIsPlayingInteraction)
	{
		return;
	}

	Super::ActionEventByPressedKey();
	FindWeaken();
}

void AMissionItemWeakenBoss::ActionBeginOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionBeginOverlap();

	PrintMissionActiveText();
	InteractionWidget->VisibleNotify(true);
}

void AMissionItemWeakenBoss::ActionEndOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionEndOverlap();

	InteractionWidget->VisibleNotify(false);
}

void AMissionItemWeakenBoss::FindWeaken()
{
	MissionHandle->ApplyBossWeaken();
	PrintMissionClearText();
	SetIsActive(false);
}
