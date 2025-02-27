// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemNPCEquipment.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

void AMissionItemNPCEquipment::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(false);
}

void AMissionItemNPCEquipment::ActionBeginOverlap()
{
	if (bIsPlayingInteraction)
	{
		return;
	}

	Super::ActionBeginOverlap();

	InteractionWidget->VisibleNotify(true);
	PrintMissionActiveText();
}

void AMissionItemNPCEquipment::ActionEndOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionEndOverlap();

	InteractionWidget->VisibleNotify(false);
}

void AMissionItemNPCEquipment::ActionEventByPressedKey()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionEventByPressedKey();

	MissionHandle->ApplyNpcEquip();
	PrintMissionClearText();
	SetVisible(false);
	SetIsActive(false);
}
