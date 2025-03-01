// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemDefenceArea.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

void AMissionItemDefenceArea::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(false);
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

	if (CurrentTime >= TargetTime)
	{
		CurrentTime = 0.0f;
		ProgressValue -= 0.01f;
		InteractionWidget->UpdateProgressBar(ProgressValue);

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
	InteractionWidget->VisibleProgressBar(true);
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
	InteractionWidget->VisibleProgressBar(false);
}

void AMissionItemDefenceArea::CompleteProgress()
{
	Super::CompleteProgress();

	SetActorTickEnabled(false);
	MissionHandle->CompleteMission();
}
