#include "Boss/State/Boss_Skill2.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Boss/Boss_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UBoss_Skill2::UBoss_Skill2()
{
}

void UBoss_Skill2::EnterState(ABoss* Boss)
{
    Super::EnterState(Boss);
    UE_LOG(LogTemp, Log, TEXT("Boss is entering SKILL2"));

    BossRef = Boss;
    if (!BossRef)
    {
        return;
    }

    if (BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->StopMovementImmediately();
    }

    PlaySkill2Animation();

    UWorld* World = BossRef->GetWorld();
    if (World)
    {
        FTimerHandle SpawnTimerHandle;
        World->GetTimerManager().SetTimer(SpawnTimerHandle, [this]()
            {
                SpawnMonsters();
                BossRef->SetState(EBossState::Idle);
            }, 2.0f, false);
    }
}


void UBoss_Skill2::TickState(float DeltaTime)
{
}

void UBoss_Skill2::ExitState()
{
    UE_LOG(LogTemp, Log, TEXT("Boss is exiting SKILL2"));
    BossRef = nullptr;
}

void UBoss_Skill2::PlaySkill2Animation()
{
    USkeletalMeshComponent* Mesh = BossRef->GetMesh();
    if (!Mesh)
    {
        return;
    }
    else
    {
        UAnimInstance* AnimInst = Mesh->GetAnimInstance();
        if (!AnimInst)
        {
            return;
        }
        else
        {
            UBoss_AnimInstance* BossAnimInst = Cast<UBoss_AnimInstance>(AnimInst);
            if (BossAnimInst && BossAnimInst->Skill2Montage)
            {
                if (AnimInst->Montage_IsPlaying(BossAnimInst->Skill2Montage))
                {
                    AnimInst->Montage_Stop(0.1f, BossAnimInst->Skill2Montage);
                }

                float Duration = AnimInst->Montage_Play(BossAnimInst->Skill2Montage);
            }
        }
    }
}

void UBoss_Skill2::SpawnMonsters()
{
    if (!BossRef) return;

    UWorld* World = BossRef->GetWorld();
    if (!World)
    {
        return;
    }

    for (int32 i = 0; i < BossRef->Skill2SpawnCount; i++)
    {
        int32 RandomIndex = FMath::RandRange(0, BossRef->Skill2MonsterList.Num() - 1);
        TSubclassOf<AActor> MonsterClass = BossRef->Skill2MonsterList[RandomIndex];
        if (!MonsterClass)
        {
            continue;
        }

        float Distance = FMath::RandRange(1000.0f, BossRef->Skill2SpawnRadius);
        FVector RandomDirection = FMath::VRand();
        RandomDirection.Z = 0.f;
        RandomDirection.Normalize();
        FVector SpawnLocation = BossRef->GetActorLocation() + RandomDirection * Distance;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        AActor* SpawnedMonster = World->SpawnActor<AActor>(MonsterClass, SpawnLocation, SpawnRotation);
    }
}
