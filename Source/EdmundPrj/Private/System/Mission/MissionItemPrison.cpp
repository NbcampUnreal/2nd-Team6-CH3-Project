// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemPrison.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

void AMissionItemPrison::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type, const FString& MissionInfo)
{
	Super::InitMissionItem(NewMissionHandle, Type, MissionInfo);

	ApplyOverlapCollision(true);
	SetActorTickEnabled(false);
}

void AMissionItemPrison::ActionEventByPressedKey()
{
	if (bIsPlayingInteraction)
	{
		return;
	}

	Super::ActionEventByPressedKey();

	InteractionWidget->VisibleProgressBar(true);
}

AMissionItemPrison::AMissionItemPrison() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMissionItemPrison::ActionBeginOverlap()
{
	if (!bIsStarted)
	{
		return;
	}

	Super::ActionBeginOverlap();

	InteractionWidget->VisibleNotify(true);
	SetActorTickEnabled(true);
}

void AMissionItemPrison::ActionEndOverlap()
{
	if (!bIsStarted)
	{
		return;
	}

	Super::ActionEndOverlap();

	InteractionWidget->VisibleNotify(false);
	InteractionWidget->VisibleProgressBar(false);

	SetActorTickEnabled(false);
}

void AMissionItemPrison::Tick(float DeltaTime)
{
	if (!bIsPlayingInteraction)
	{
		return;
	}

	CurrentTime += DeltaTime;

	if (CurrentTime >= TargetTime)
	{
		CurrentTime = 0.0f;
		ProgressValue -= 0.1f;
		InteractionWidget->UpdateProgressBar(ProgressValue);

		if (ProgressValue <= 0.0f)
		{
			CompleteProgress();
		}
	}
}

void AMissionItemPrison::CompleteProgress()
{
	SetActorTickEnabled(false);
	InteractionWidget->VisibleProgressBar(false);

	UpdateNotifyTextToUI();
	//MissionHandle->CompleteMission();
}


