// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RangedMonster.h"
#include "Monster/RangedMonsterBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Monster/MonsterSpawner.h"
#include "Engine/World.h"

ARangedMonster::ARangedMonster()
{
    MonsterType = EMonsterType::Ranger;
}

void ARangedMonster::BeginPlay()
{
    Super::BeginPlay();
}

void ARangedMonster::MonsterAttackCheck()
{
    Fire();
}

void ARangedMonster::Fire()
{
    PlayParticle();
    PlaySound();

    if (MonsterSpawner)
    {
        ARangedMonsterBullet* BulletToFire = MonsterSpawner->GetBulletFromSpawner();
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
                UE_LOG(LogTemp, Warning, TEXT("BulletToFire 없음. GameMode Pool에 있는 GetBulletFromPool이 없을 수 있음"));
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
        FVector ParticleScale = FVector(1.0f, 1.0f, 1.0f);
        FVector ParticleLocation = GetActorLocation() + GetActorForwardVector() * 50.0f + GetActorRightVector() * 80.0f;

        Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            AttackParticle,
            ParticleLocation,
            GetActorRotation(),
            ParticleScale,
            false
        );
    }
}

void ARangedMonster::PlaySound()
{
    GameState->PlayMonsterSound(CurrentAudioComp, MonsterType, ESoundType::Attack);
    //CurrentAudioComp->SetSound(AttackSound);
    //CurrentAudioComp->Play();
}



