// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBulletPool.h"
#include "Monster/RangedMonsterBullet.h"

// Sets default values
AMonsterBulletPool::AMonsterBulletPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}
// Called when the game starts or when spawned
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

            if (!NewBullet)
            {
                UE_LOG(LogTemp, Error, TEXT("There is No Bullet"));
            }

            if (NewBullet)
            {
                NewBullet->SetActorHiddenInGame(true);
                BulletPool.Add(NewBullet);
                UE_LOG(LogTemp, Warning, TEXT("TT"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MonsterBulletClass 없음"));
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

        UE_LOG(LogTemp, Warning, TEXT("모든 Bullet이 Hidden이 아님"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CC"));
    }

    UE_LOG(LogTemp, Error, TEXT("There is no MonsterBullet in the object pool."));
    return nullptr;;
}
