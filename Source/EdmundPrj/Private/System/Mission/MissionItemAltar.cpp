// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemAltar.h"
#include "System/MissionHandle.h"
#include "UI/3DWidget/InteractionWidget.h"

void AMissionItemAltar::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyBlockCollision();
	MissionHandle->AddAlter(this);
}

float AMissionItemAltar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bIsActive)
	{
		return 0.0f;
	}

	--HitCount;

	if (HitCount <= 0)
	{
		ApplyDestroyedEvent();
	}

	return 0.0f;
}

void AMissionItemAltar::ApplyDestroyedEvent()
{
	MissionHandle->LockToBossMonsterSkill(this);
	PrintMissionClearText();
	SetVisible(false);
}
