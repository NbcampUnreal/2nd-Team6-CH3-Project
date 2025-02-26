// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemIndicator.h"

void AMissionItemIndicator::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type, const FString& MissionInfo)
{
	Super::InitMissionItem(NewMissionHandle, Type, MissionInfo);

	ApplyBlockCollision();
}
