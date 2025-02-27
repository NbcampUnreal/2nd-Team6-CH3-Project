// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemTargetPoint.h"
#include "System/MissionHandle.h"

void AMissionItemTargetPoint::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(false);

	MissionHandle->SetTargetPointLocation(GetActorLocation());
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
	PrintMissionActiveText();
}
