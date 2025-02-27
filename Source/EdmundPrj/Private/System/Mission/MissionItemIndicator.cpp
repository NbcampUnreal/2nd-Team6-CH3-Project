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
	//방향 표시
	SetVisible(false);
}
