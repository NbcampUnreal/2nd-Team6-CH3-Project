// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemDimensionPortal.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

void AMissionItemDimensionPortal::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(true);
	MissionHandle->AddDimentionPortalSet(this);
}
