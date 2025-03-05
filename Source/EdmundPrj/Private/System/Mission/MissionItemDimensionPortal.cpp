// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemDimensionPortal.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

AMissionItemDimensionPortal::AMissionItemDimensionPortal() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>("GateMesh");
	GateMesh->SetupAttachment(RootComponent);
}

void AMissionItemDimensionPortal::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(false);
	MissionHandle->AddDimensionPortalSet(this);
	SetActorTickEnabled(false);

	MeshStartPos = GateMesh->GetComponentLocation();
	AddPos = FVector(0, 0, -MeshStartPos.Z / 3 * 2);
	DeltaTargetTime = 1.0f / TargetTime;
	AddPos = AddPos * DeltaTargetTime;
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
		GateMesh->AddLocalOffset(AddPos * UpdateTime);

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
	InteractionWidget->UpdateProgressBar(1.0f);
	ProgressValue = 1.0f;
	GateMesh->SetWorldLocation(MeshStartPos);
}

void AMissionItemDimensionPortal::CompleteProgress()
{
	Super::CompleteProgress();

	SetIsActive(false);
	SetActorTickEnabled(false);
	MissionHandle->RemoveDimensionPortalSet(this);
	PrintMissionClearText();
}
