// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/IndicatorParts.h"
#include "System/MissionHandle.h"

AIndicatorParts::AIndicatorParts() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AIndicatorParts::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(false);
	SetIsActive(true);

	Target = MissionHandle->GetPrison();
	StartHeight = GetActorLocation().Z;
}

void AIndicatorParts::Tick(float DeltaTime)
{
	if (!bIsActive)
	{
		return;
	}

	Super::Tick(DeltaTime);

	if (!bIsMove)
	{
		if (GetActorLocation().Z <= StartHeight + TargetHeightAmount)
		{
			AddActorWorldOffset(GetActorUpVector() * MoveUpSpeed);
			return;
		}
		else
		{
			bIsMove = true;
			Direction = Target->GetActorLocation() - GetActorLocation();
			Direction = Direction / Direction.Size();
		}
	}

	AddActorWorldOffset(Direction * MoveForwardSpeed);
}

void AIndicatorParts::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (OtherActor == Target)
	{
		Destroy();
	}
}
