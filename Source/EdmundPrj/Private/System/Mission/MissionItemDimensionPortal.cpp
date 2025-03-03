// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemDimensionPortal.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

AMissionItemDimensionPortal::AMissionItemDimensionPortal() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMissionItemDimensionPortal::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(true);
	MissionHandle->AddDimensionPortalSet(this);
	SetActorTickEnabled(false);

	DeltaTargetTime = 1.0f / TargetTime;
}

void AMissionItemDimensionPortal::ActionEventByPressedKey()
{
	if (bIsPlayingInteraction)
	{
		return;
	}

	Super::ActionEventByPressedKey();

	InteractionWidget->VisibleProgressBar(true);
	SetActorTickEnabled(true);
}

void AMissionItemDimensionPortal::Tick(float DeltaTime)
{
	if (!bIsPlayingInteraction)
	{
		return;
	}

	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;

	if (CurrentTime >= UpdateTime)
	{
		CurrentTime = 0;
		ProgressValue -= DeltaTargetTime * UpdateTime;
		InteractionWidget->UpdateProgressBar(ProgressValue);

		if (ProgressValue <= 0)
		{
			CompleteProgress();
		}
	}
}

void AMissionItemDimensionPortal::ActionBeginOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionBeginOverlap();

	InteractionWidget->VisibleNotify(true);
	PrintMissionActiveText();
}

void AMissionItemDimensionPortal::ActionEndOverlap()
{
	if (!bIsActive)
	{
		return;
	}

	Super::ActionEndOverlap();

	SetActorTickEnabled(false);
	InteractionWidget->VisibleNotify(false);
	InteractionWidget->VisibleProgressBar(false);
}

void AMissionItemDimensionPortal::CompleteProgress()
{
	Super::CompleteProgress();

	SetIsActive(false);
	SetActorTickEnabled(false);
	MissionHandle->RemoveDimensionPortalSet(this);
	PrintMissionClearText();
}
