// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMonster.h"
#include "MonsterSpawner.generated.h"

class UBoxComponent;
class AMonsterBulletPool;
class ARangedMonsterBullet;
class ASpawnerHandle;

UCLASS()
class EDMUNDPRJ_API AMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawner();

	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void ClearTimer();

	void BossSpawn(ASpawnerHandle* NewSpawnerHandle, AMonsterBulletPool* BulletPool, int32 NewSpawnCount);

	void SetBossMode(bool NewMode);

	bool bCheckAllDead();

	void AddDeadCount();

	void DestroySpawner();

	void ApplyChaseMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Spawn")
	int32 DeadMonsterCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Spawn")
	bool bBossMode = false;


	AGameModeBase* CurrentGameMode;

	void InitSpawner(AMonsterBulletPool* BulletPool, float NewSpawnTime, int32 NewSpawnCount, int32 NewLevelIndex);

	void InitSpawner(AMonsterBulletPool* BulletPool, float NewSpawnTime, int32 NewSpawnCount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effects")
	UParticleSystem* SpawnParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Bullet")
	AMonsterBulletPool* MonsterBulletPool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Spawn")
	TArray<ABaseMonster*> SpawnedMonstersPool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Spawn")
	TArray<TSubclassOf<ABaseMonster>> AllMonsters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Spawn")
	TSubclassOf<ABaseMonster> MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Component")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Component")
	UBoxComponent* SpawnVolumeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Component")
	UStaticMeshComponent* SpawnerMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Value")
	float SpawnTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Value")
	int32 SpawnCount = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster|Value")
	int32 LevelIndex;

	ABaseMonster* GetMonsterFromPool();

	ARangedMonsterBullet* GetBulletFromSpawner();

	void InitializeMonsterSpawnPool(int32 PoolSize);

	FTimerHandle SpawnTimerHandle;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnMonster();

	FVector GetSpawnVolume();

	TObjectPtr<ASpawnerHandle> SpawnerHandle;
	int32 BossModeSpawnCount = 0;
	bool bIsDefenceMode = false;
};
