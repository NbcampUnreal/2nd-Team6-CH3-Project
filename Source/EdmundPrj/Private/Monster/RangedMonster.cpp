// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RangedMonster.h"
#include "Monster/RangedMonsterBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Monster/MonsterSpawner.h"
#include "Player/PlayerCharacter.h"
#include "Engine/World.h"

// 몬스터 스탯 설정은 여기서!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void ARangedMonster::SetMonsterStatsByLevel()
{

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (IsValid(PlayerController))
    {
        APawn* PlayerPawn = PlayerController->GetPawn();
        if (IsValid(PlayerPawn))
        {
            ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(PlayerPawn);
            if (PlayerCharacter)
            {
                MonsterHP = 50 + (MonsterLevel * 30);
                MonsterMaxHP = 50 + (MonsterLevel * 30);
                MonsterAttackDamage = 10.0f + (MonsterLevel * 5.0f);
                MonsterArmor = 5.0f + (MonsterLevel * 2.0f);
                MonsterExpReward += 50 * (PlayerCharacter->GetExpMultipler() - 100) / 100;
                MonsterGoldReward += MonsterGoldReward * (PlayerCharacter->GetGoldMultipler() - 100) / 100;
                MonsterHealKitProbability = PlayerCharacter->GetItempDropProb() / 2;
                MonsterGoldProbability = PlayerCharacter->GetItempDropProb();
            }
        }
    }
}

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
        }
    }
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
}



