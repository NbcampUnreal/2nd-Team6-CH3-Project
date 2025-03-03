// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemPrison.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

void AMissionItemPrison::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(true);
	SetActorTickEnabled(false);

	MissionHandle->SetPrison(this);
	MissionHandle->SpawnNpc(GetActorLocation());
	MissionHandle->SetNpcBondageMode(true);

	DeltaTargetTime = 1.0f / TargetTime;
}

void AMissionItemPrison::ActionEventByPressedKey()
{
	if (bIsPlayingInteraction)
	{
		return;
	}

	Super::ActionEventByPressedKey();

	InteractionWidget->VisibleProgressBar(true);
	SetActorTickEnabled(true);
}

AMissionItemPrison::AMissionItemPrison() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMissionItemPrison::ActionBeginOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionBeginOverlap();

	ProgressValue = 1.0f;
	InteractionWidget->UpdateProgressBar(ProgressValue);
	InteractionWidget->VisibleNotify(true);
	PrintMissionActiveText();
}

void AMissionItemPrison::ActionEndOverlap()
{
	if (!bIsActive)
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

	if (CurrentTime >= UpdateTime)
	{
		CurrentTime = 0.0f;
		ProgressValue -= DeltaTargetTime * UpdateTime;
		InteractionWidget->UpdateProgressBar(ProgressValue);

		if (ProgressValue <= 0.0f)
		{
			CompleteProgress();
		}
	}
}

void AMissionItemPrison::CompleteProgress()
{
	Super::CompleteProgress();

	SetVisible(false);
	MissionHandle->SetNpcBondageMode(false);
	MissionHandle->SetNpcMoveMode(true);
	MissionHandle->CompleteMission();
}


