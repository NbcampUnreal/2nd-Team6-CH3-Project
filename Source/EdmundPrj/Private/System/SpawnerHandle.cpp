// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SpawnerHandle.h"
#include "System/EdmundGameMode.h"
#include "System/EdmundGameState.h"
#include "Monster/MonsterSpawner.h"
#include "System/DataStructure/SpawnerDataRow.h"
#include "Monster/MonsterBulletPool.h"

// Sets default values
ASpawnerHandle::ASpawnerHandle()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawnerHandle::InitSpawnerHandle(AEdmundGameMode* NewGameMode, AEdmundGameState* NewGameState, const TArray<FSpawnerDataRow*>& CurrentSpawnerData)
{
	EdmundGameMode = NewGameMode;
	EdmundGameState = NewGameState;

	if (CurrentSpawnerData.Num() == 0)
	{
		return;
	}

	SpawnerDataSet = CurrentSpawnerData;
}

void ASpawnerHandle::ApplySpawnerDataInLevel()
{
	checkf(IsValid(MonsterBulletPoolClass), TEXT("Monster Bullet Pool Class is invalid"));

	MonsterBulletPool = GetWorld()->SpawnActor<AMonsterBulletPool>(MonsterBulletPoolClass);
	MonsterBulletPool->InitializeMonsterBulletPool(10);

	for (const FSpawnerDataRow* SpawnerDataRow : SpawnerDataSet)
	{
		UClass* SpawnClass = SpawnerDataRow->SpawnerClass.Get();

		for (const FVector& SpawnPos : SpawnerDataRow->SpawnLocationSet)
		{
			SpawnMonsterSpawner(SpawnClass, SpawnPos);
		}
	}
}

void ASpawnerHandle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpawnerHandle::SpawnMonsterSpawner(UClass* SpawnClass, const FVector& SpawnPos)
{
	checkf(IsValid(SpawnClass), TEXT("Spawner Spawn Class is invalid"));

	FActorSpawnParameters SpawnParam;

	AMonsterSpawner* NewMonsterSpawner = GetWorld()->SpawnActor<AMonsterSpawner>(SpawnClass, SpawnPos, FRotator::ZeroRotator, SpawnParam);

	MonsterSpawnerSet.Add(NewMonsterSpawner);
}


