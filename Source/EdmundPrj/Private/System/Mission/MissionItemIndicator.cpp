// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemIndicator.h"

void AMissionItemIndicator::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyBlockCollision();
}

float AMissionItemIndicator::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bIsActive)
	{
		return 0.0;
	}

	--HitCount;

	if (HitCount <= 0)
	{
		bIsActive = false;
		PrintMissionClearText();
		ShowDirectionToTarget();
	}

	return 0.0f;
}

void AMissionItemIndicator::ShowDirectionToTarget()
{
	if (!IsValid(PartsClass))
	{
		return;
	}

	FActorSpawnParameters SpawnParam;
	FVector SpawnPos = GetActorLocation() + GetActorUpVector() * 200;

	IndicatorParts = GetWorld()->SpawnActor<ABaseMissionItem>(PartsClass, SpawnPos, FRotator::ZeroRotator, SpawnParam);
	
	if (IsValid(IndicatorParts))
	{
		IndicatorParts->InitMissionItem(MissionHandle, "Parts");
	}

	SetVisible(false);
}
