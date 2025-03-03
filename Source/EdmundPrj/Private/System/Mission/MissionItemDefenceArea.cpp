// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemDefenceArea.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

void AMissionItemDefenceArea::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(false);

	DeltaTargetTime = 1.0f / TargetTime;
	ProgressValue = 1.0f;
	MissionHandle->UpdateDefenceProgress(ProgressValue);
}

void AMissionItemDefenceArea::SetIsActive(bool Value)
{
	Super::SetIsActive(Value);

	SetVisible(Value);
	MissionHandle->NotifyStartDefenceMode();
}

AMissionItemDefenceArea::AMissionItemDefenceArea() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMissionItemDefenceArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsPlayingInteraction)
	{
		return;
	}

	CurrentTime += DeltaTime;

	if (CurrentTime >= UpdateTime)
	{
		CurrentTime = 0.0f;
		ProgressValue -= DeltaTargetTime * UpdateTime;
		MissionHandle->UpdateDefenceProgress(ProgressValue);

		if (ProgressValue <= 0.0f)
		{
			CompleteProgress();
		}
	}
}

void AMissionItemDefenceArea::ActionBeginOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionBeginOverlap();

	bIsPlayingInteraction = true;
	SetActorTickEnabled(true);
	MissionHandle->UpdateDefenceState(true);
	PrintMissionActiveText();
}

void AMissionItemDefenceArea::ActionEndOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionEndOverlap();

	SetActorTickEnabled(false);
	MissionHandle->UpdateDefenceState(false);
}

void AMissionItemDefenceArea::CompleteProgress()
{
	Super::CompleteProgress();

	SetActorTickEnabled(false);
	MissionHandle->CompleteMission();
}
