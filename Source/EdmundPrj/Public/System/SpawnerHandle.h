// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System/EnumSet.h"
#include "SpawnerHandle.generated.h"

class AEdmundGameMode;
class AEdmundGameState;
class AMonsterSpawner;
class AMonsterBulletPool;
struct FSpawnerDataRow;

UCLASS()
class EDMUNDPRJ_API ASpawnerHandle : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnerHandle();
	void InitSpawnerHandle(AEdmundGameMode* NewGameMode, AEdmundGameState* NewGameState, const TArray<FSpawnerDataRow*>& CurrentSpawnerData);
	void ApplySpawnerDataInLevel();
	void ApplyDefenceMode();
	void DestroyAllSpawner();
	void SpawnBossPatternSpawner(const TArray<FVector>& PosSet);
	void IncreaseSpawnerClearCount();
	void ClearSpawnPattern();

private:
	virtual void BeginPlay() override;
	void SpawnMonsterSpawner(UClass* SpawnClass, const FVector& SpawnPos, const float SpawnTime, const int32 SpawnCount);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectPool")
	TSubclassOf<AMonsterBulletPool> MonsterBulletPoolClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectPool")
	TSubclassOf<AMonsterSpawner> NormalSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectPool")
	TSubclassOf<AMonsterSpawner> SuperSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnInfo")
	int32 BossPatternSpawnCount = 5;

private:
	TObjectPtr<AEdmundGameMode> EdmundGameMode;
	TObjectPtr<AEdmundGameState> EdmundGameState;
	TObjectPtr<AMonsterBulletPool> MonsterBulletPool;

	TArray<FSpawnerDataRow*> SpawnerDataSet;
	TArray<AMonsterSpawner*> MonsterSpawnerSet;

	ESceneType CurrentMission = ESceneType::Mission1;

	int32 SpawnerClearCount = 0;
};
