// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBulletPool.h"
#include "Monster/RangedMonsterBullet.h"

AMonsterBulletPool::AMonsterBulletPool()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMonsterBulletPool::BeginPlay()
{
	Super::BeginPlay();

    InitializeMonsterBulletPool(BulletPoolSize);
}

void AMonsterBulletPool::InitializeMonsterBulletPool(int32 PoolSize)
{
    FRotator SpawnRotation(GetActorRotation());
    FVector SpawnLocation(GetActorLocation());
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    for (int32 i = 0; i < PoolSize; i++)
    {
        SpawnLocation.Z -= 500;
        if (MonsterBulletClass)
        {
            ARangedMonsterBullet* NewBullet = GetWorld()->SpawnActor<ARangedMonsterBullet>(MonsterBulletClass, SpawnLocation, SpawnRotation, SpawnParams);

            if (NewBullet)
            {
                NewBullet->SetActorHiddenInGame(true);
                BulletPool.Add(NewBullet);
            }
        }
    }
}

ARangedMonsterBullet* AMonsterBulletPool::GetBulletFromPool()
{
    if (BulletPool.Num() > 0)
    {
        for (ARangedMonsterBullet* Bullet : BulletPool)
        {
            if (Bullet && Bullet->IsHidden())
            {
                Bullet->SetMonsterBulletHidden(false);
                return Bullet;
            }
        }
    }

    return nullptr;;
}
