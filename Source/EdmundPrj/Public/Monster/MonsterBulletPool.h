// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterBulletPool.generated.h"

class ARangedMonsterBullet;

UCLASS()
class EDMUNDPRJ_API AMonsterBulletPool : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterBulletPool();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Bullet")
	int32 BulletPoolSize = 30;

	void BeginPlay() override;

	void InitializeMonsterBulletPool(int32 PoolSize);

	ARangedMonsterBullet* GetBulletFromPool();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Bullet")
	TSubclassOf<ARangedMonsterBullet> MonsterBulletClass;

	UPROPERTY()
	TArray<ARangedMonsterBullet*> BulletPool;

private:
};
