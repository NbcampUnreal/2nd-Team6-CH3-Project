// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RangedMonster.h"
#include "Monster/RangedMonsterBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

ARangedMonster::ARangedMonster()
{
}

void ARangedMonster::BeginPlay()
{
    Super::BeginPlay();
    InitializeMonsterBulletPool(10);
}

void ARangedMonster::MonsterAttackCheck()
{
    USkeletalMeshComponent* MeshComp = GetMesh();
    ABaseMonster* Monster = Cast<ABaseMonster>(this);

    Fire();

}

void ARangedMonster::InitializeMonsterBulletPool(int32 PoolSize)
{
    FRotator SpawnRotation(0, 0, 0);
    FVector SpawnLocation(0, 0, 0);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    for (int32 i = 0; i < PoolSize; i++)
    {
        SpawnLocation.X += 2000;

        ARangedMonsterBullet* NewBullet = GetWorld()->SpawnActor<ARangedMonsterBullet>(MonsterBulletClass, SpawnLocation, SpawnRotation, SpawnParams);

        if (!NewBullet)
        {
            UE_LOG(LogTemp, Error, TEXT("There is No Bullet"))
        }

        if (NewBullet)
        {
            NewBullet->SetActorHiddenInGame(true);
            BulletPool.Add(NewBullet);
        }
    }
}

ARangedMonsterBullet* ARangedMonster::GetBulletFromPool()
{
    for (ARangedMonsterBullet* Bullet : BulletPool)
    {
        if (Bullet && Bullet->IsHidden())
        {
            Bullet->SetMonsterBulletHidden(false);
            return Bullet;
        }
    }

    UE_LOG(LogTemp, Error, TEXT("There is no MonsterBullet in the object pool."));
    return nullptr;
}

void ARangedMonster::Fire()
{
    PlayParticle();
    PlaySound();

    ARangedMonsterBullet* BulletToFire = GetBulletFromPool();

    if (BulletToFire)
    {
        FVector SpawnLocation = this->GetActorLocation();
        FRotator SpawnRotation = this->GetActorRotation();

        BulletToFire->SetDamage(GetMonsterAttackDamage());

        BulletToFire->SetActorLocation(SpawnLocation);
        BulletToFire->SetActorRotation(SpawnRotation);

        FVector ForwardDirection = SpawnRotation.Vector();

        if (BulletToFire->ProjectileMovementComp)
        {
            BulletToFire->ProjectileMovementComp->Velocity = ForwardDirection * BulletToFire->ProjectileMovementComp->InitialSpeed;
        }
    }
}



void ARangedMonster::PlayParticle()
{
    //파티클 활성화, 헤더는 BaseMonster에 있음
    UParticleSystemComponent* Particle = nullptr;

    if (AttackParticle)
    {
        FVector ParticleScale = FVector(8.0f, 8.0f, 8.0f);

        Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            AttackParticle,
            GetActorLocation(),
            GetActorRotation(),
            ParticleScale,
            false
        );
    }
}

void ARangedMonster::PlaySound()
{
    CurrentAudioComp->SetSound(AttackSound);
    CurrentAudioComp->Play();
}



