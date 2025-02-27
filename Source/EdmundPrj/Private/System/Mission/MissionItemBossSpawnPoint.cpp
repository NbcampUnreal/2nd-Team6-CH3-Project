// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/MissionItemBossSpawnPoint.h"
#include "System/MissionHandle.h"
#include "Boss/Boss.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

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
	}
}

void AMissionItemBossSpawnPoint::ClearBoss()
{
	MissionHandle->CompleteMission();
}

void AMissionItemBossSpawnPoint::SpawnBoss()
{
	//AActor* Enemy_Instance = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), EnemyPawnExplosion, EnemyBehaviorTree, GetActorLocation(), GetActorRotation(), true);
	UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), BossActor, BossBt, GetActorLocation(), GetActorRotation(), true);
}
