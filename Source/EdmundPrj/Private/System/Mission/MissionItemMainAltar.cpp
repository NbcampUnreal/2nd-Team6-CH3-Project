// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemMainAltar.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"


AMissionItemMainAltar::AMissionItemMainAltar() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMissionItemMainAltar::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(true);
	SetActorTickEnabled(false);

	DeltaTargetTime = 1.0f / TargetTime;
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
	PrintMissionActiveText();
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

void AMissionItemMainAltar::CompleteProgress()
{
	Super::CompleteProgress();

	PrintMissionClearText();
	SetVisible(false);
	MissionHandle->RequestSwapBgm(EBGMSoundType::Boss);
	MissionHandle->CompleteMission();
}
