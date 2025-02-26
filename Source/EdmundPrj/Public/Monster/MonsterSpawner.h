// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMonster.h"
#include "MonsterSpawner.generated.h"

class UBoxComponent;
class AMonsterBulletPool;
class ARangedMonsterBullet;

UCLASS()
class EDMUNDPRJ_API AMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawner();

	AGameModeBase* CurrentGameMode;

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

	ABaseMonster* GetMonsterFromPool();

	ARangedMonsterBullet* GetBulletFromSpawner();

	void InitializeMonsterSpawnPool(int32 PoolSize);

	FTimerHandle SpawnTimerHandle;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnMonster();

	FVector GetSpawnVolume();

};
