// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemTargetPoint.h"
#include "System/MissionHandle.h"

void AMissionItemTargetPoint::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type, const FString& MissionInfo)
{
	Super::InitMissionItem(NewMissionHandle, Type, MissionInfo);

	ApplyOverlapCollision(false);
}

void AMissionItemTargetPoint::ActionBeginOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionBeginOverlap();

	bIsActive = false;
	SetVisible(false);
	MissionHandle->CompleteMission();
}
