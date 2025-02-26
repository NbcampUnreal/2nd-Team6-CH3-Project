// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RangedMonster.h"
#include "Monster/RangedMonsterBullet.h"
#include "Monster/MonsterBulletPool.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/World.h"

ARangedMonster::ARangedMonster()
{
}

void ARangedMonster::BeginPlay()
{
    Super::BeginPlay();

    //   AGameModeBase* CurrentGameMode = GetWorld()->GetAuthGameMode();
    //   if (CurrentGameMode)
    //   {
       //	MonsterBulletPool = CurrentGameMode->GetMonsterBulletPool();
       //}
    //   else
    //   {
    //       UE_LOG(LogTemp, Warning, TEXT("CurrentGameMode 없음"));
    //   }
}

void ARangedMonster::MonsterAttackCheck()
{
    Fire();
}

void ARangedMonster::Fire()
{
    PlayParticle();
    PlaySound();

    if (MonsterBulletPool)
    {
        ARangedMonsterBullet* BulletToFire = MonsterBulletPool->GetBulletFromPool();
        {
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
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("BulletToFire 없음"));
            }
        }
    }
    //else if (!MonsterBulletPool)
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("MonsterBUlletPool 없음"));
    //}
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



