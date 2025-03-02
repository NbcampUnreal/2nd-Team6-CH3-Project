// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemBossSpawnPoint.h"
#include "System/MissionHandle.h"
#include "Boss/Boss.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Boss/BossAIController.h"

void AMissionItemBossSpawnPoint::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type)
{
	Super::InitMissionItem(NewMissionHandle, Type);

	ApplyOverlapCollision(false);
}

void AMissionItemBossSpawnPoint::SetIsActive(bool Value)
{
	Super::SetIsActive(Value);

	if (Value)
	{
		SpawnBoss();
		PrintMissionActiveText();
		MissionHandle->NotifyStartedBossStage(this);
	}
}

void AMissionItemBossSpawnPoint::SpawnMonster() const
{
	MissionHandle->RequestSpawnToSpawnerHandle();
}

void AMissionItemBossSpawnPoint::RequestMoveToNextPattern()
{
	ABossAIController* BossController = BossPawn->GetController<ABossAIController>();

	if (IsValid(BossController))
	{
		BossController->NotifyClearHalfPattern();
	}
}

void AMissionItemBossSpawnPoint::SpawnBoss()
{
	if (!IsValid(BossClass))
	{
		return;
	}

	if (!IsValid(BossBt))
	{
		return;
	}

	APawn* NewBoss = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), BossClass, BossBt, GetActorLocation(), GetActorRotation(), true);
	
	BossPawn = Cast<ABoss>(NewBoss);

	BossPawn->InitBoss(MissionHandle);
}
