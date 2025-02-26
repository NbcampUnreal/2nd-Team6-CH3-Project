// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemMainAltar.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"


AMissionItemMainAltar::AMissionItemMainAltar() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMissionItemMainAltar::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type, const FString& MissionInfo)
{
	Super::InitMissionItem(NewMissionHandle, Type, MissionInfo);

	ApplyOverlapCollision(true);
	SetActorTickEnabled(false);
}

void AMissionItemMainAltar::ActionEventByPressedKey()
{
	if (bIsPlayingInteraction)
	{
		return;
	}

	Super::ActionEventByPressedKey();

	InteractionWidget->VisibleProgressBar(true);
	SetActorTickEnabled(true);
}

void AMissionItemMainAltar::ActionBeginOverlap()
{
	Super::ActionBeginOverlap();

	ProgressValue = 1.0f;
	InteractionWidget->UpdateProgressBar(ProgressValue);
	InteractionWidget->VisibleNotify(true);
}

void AMissionItemMainAltar::ActionEndOverlap()
{
	Super::ActionEndOverlap();

	InteractionWidget->VisibleNotify(false);
	InteractionWidget->VisibleProgressBar(false);

	SetActorTickEnabled(false);
}

void AMissionItemMainAltar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsPlayingInteraction)
	{
		return;
	}

	CurrentTime += DeltaTime;

	if (CurrentTime >= TargetTime)
	{
		CurrentTime = 0.0f;
		ProgressValue -= TargetTime;
		InteractionWidget->UpdateProgressBar(ProgressValue);

		if (ProgressValue <= 0.0f)
		{
			CompleteProgress();
		}
	}
}

void AMissionItemMainAltar::CompleteProgress()
{
	Super::CompleteProgress();

	SetVisible(false);
	MissionHandle->CompleteMission();
}
