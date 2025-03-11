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

	CurrentMission = SpawnerDataSet[0]->InSceneType;

	for (const FSpawnerDataRow* SpawnerDataRow : SpawnerDataSet)
	{
		UClass* SpawnClass = SpawnerDataRow->SpawnerClass.Get();

		for (int32 i = 0; i < SpawnerDataRow->SpawnLocationSet.Num(); i++)
		{
			FVector SpawnPos = SpawnerDataRow->SpawnLocationSet[i];
			float SpawnTime = SpawnerDataRow->SpawnTimeSet[i];
			int32 SpawnCount = SpawnerDataRow->SpawnCountSet[i];

			SpawnMonsterSpawner(SpawnClass, SpawnPos, SpawnTime, SpawnCount);
		}
	}
}

void ASpawnerHandle::ApplyDefenceMode()
{
	for (AMonsterSpawner* Spawner : MonsterSpawnerSet)
	{
		Spawner->ApplyChaseMode();
	}
}

void ASpawnerHandle::DestroyAllSpawner()
{
	for (AMonsterSpawner* Spawner : MonsterSpawnerSet)
	{
		Spawner->DestroySpawner();
	}

	MonsterSpawnerSet.Empty();
}

void ASpawnerHandle::SpawnBossPatternSpawner(const TArray<FVector>& PosSet)
{
	SpawnerClearCount = PosSet.Num();

	int32 SpawnCount = 0;

	for (const FVector& SpawnPos : PosSet)
	{
		UClass* SpawnClass = nullptr;
		FVector AddVector = FVector(2000, 0, 0);

		if (SpawnCount < SpawnerClearCount / 2)
		{
			SpawnClass = NormalSpawner;
			AddVector *= -1;
		}
		else
		{
			SpawnClass = SuperSpawner;
		}

		if (!IsValid(SpawnClass))
		{
			continue;
		}

		FActorSpawnParameters SpawnParam;

		AMonsterSpawner* NewSpawner = GetWorld()->SpawnActor<AMonsterSpawner>(SpawnClass, SpawnPos + AddVector, FRotator::ZeroRotator, SpawnParam);
		NewSpawner->SetActorScale3D(FVector(1, 1, 1));
		NewSpawner->BossSpawn(this, MonsterBulletPool, BossPatternSpawnCount);

		MonsterSpawnerSet.Add(NewSpawner);
		++SpawnCount;
	}
}

void ASpawnerHandle::IncreaseSpawnerClearCount()
{
	--SpawnerClearCount;

	if (SpawnerClearCount <= 0)
	{
		ClearSpawnPattern();
	}
}

void ASpawnerHandle::ClearSpawnPattern()
{
	EdmundGameMode->NotifyClearedHalfPatternByBoss();
}

void ASpawnerHandle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpawnerHandle::SpawnMonsterSpawner(UClass* SpawnClass, const FVector& SpawnPos, const float SpawnTime, const int32 SpawnCount)
{
	checkf(IsValid(SpawnClass), TEXT("Spawner Spawn Class is invalid"));

	FActorSpawnParameters SpawnParam;

	AMonsterSpawner* NewMonsterSpawner = GetWorld()->SpawnActor<AMonsterSpawner>(SpawnClass, SpawnPos, FRotator::ZeroRotator, SpawnParam);

	int32 MissionIndex = 0;

	switch (CurrentMission)
	{
	case ESceneType::Mission1:
		MissionIndex = 0;
		break;

	case ESceneType::Mission2:
		MissionIndex = 1;
		break;

	case ESceneType::Mission3:
		MissionIndex = 2;
		break;

	case ESceneType::Infinity:
		MissionIndex = 3;
		break;

	default:
		MissionIndex = 0;
		break;
	}

	NewMonsterSpawner->InitSpawner(MonsterBulletPool, SpawnTime, SpawnCount, MissionIndex);
	MonsterSpawnerSet.Add(NewMonsterSpawner);
}


