// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemIndicator.h"

void AMissionItemIndicator::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type, const FString& MissionInfo)
{
	Super::InitMissionItem(NewMissionHandle, Type, MissionInfo);

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
		PrintMissionText();
		ShowDirectionToTarget();
	}

	return 0.0f;
}

void AMissionItemIndicator::ShowDirectionToTarget()
{
	//방향 표시
	SetVisible(false);
}
