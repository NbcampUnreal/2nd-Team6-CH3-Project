#include "Boss/State/BTTask_BossSkill2.h"
#include "Boss/Boss.h"
#include "Boss/Boss_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Engine/World.h"
#include "TimerManager.h"

UBTTask_BossSkill2::UBTTask_BossSkill2()
{
    NodeName = TEXT("Boss Skill2");
    bNotifyTick = false; // 별도의 TickTask()는 필요하지 않음
    BossRef = nullptr;
    CachedOwnerComp = nullptr;
}

EBTNodeResult::Type UBTTask_BossSkill2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        return EBTNodeResult::Failed;
    }

    BossRef = Cast<ABoss>(AICon->GetPawn());
    if (!BossRef)
    {
        return EBTNodeResult::Failed;
    }
    CachedOwnerComp = &OwnerComp;

    // 정지: 보스의 이동을 즉시 정지시킵니다.
    if (BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->StopMovementImmediately();
    }

    // 스킬 애니메이션 재생
    PlaySkill2Animation();

    // 2초 후 몬스터 소환 및 스킬 종료
    UWorld* World = BossRef->GetWorld();
    if (World)
    {
        World->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UBTTask_BossSkill2::OnSkill2Complete, 2.0f, false);
    }
    else
    {
        return EBTNodeResult::Failed;
    }

    // 태스크는 InProgress 상태로 남깁니다.
    return EBTNodeResult::InProgress;
}

void UBTTask_BossSkill2::PlaySkill2Animation()
{
    if (!BossRef) return;

    USkeletalMeshComponent* Mesh = BossRef->GetMesh();
    if (!Mesh)
    {
        return;
    }

    UAnimInstance* AnimInst = Mesh->GetAnimInstance();
    if (!AnimInst)
    {
        return;
    }

    UBoss_AnimInstance* BossAnimInst = Cast<UBoss_AnimInstance>(AnimInst);
    if (BossAnimInst && BossAnimInst->Skill2Montage)
    {
        // 이미 재생 중이면 중지 후 재생
        if (AnimInst->Montage_IsPlaying(BossAnimInst->Skill2Montage))
        {
            AnimInst->Montage_Stop(0.1f, BossAnimInst->Skill2Montage);
        }
        AnimInst->Montage_Play(BossAnimInst->Skill2Montage);
    }
}

void UBTTask_BossSkill2::SpawnMonsters()
{
    if (!BossRef) return;

    UWorld* World = BossRef->GetWorld();
    if (!World)
    {
        return;
    }

    // Boss.h에 설정된 Skill2SpawnCount, Skill2SpawnRadius, Skill2MonsterList 등을 사용하여 몬스터 소환
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

        World->SpawnActor<AActor>(MonsterClass, SpawnLocation, SpawnRotation);
    }
}

void UBTTask_BossSkill2::OnSkill2Complete()
{
    // 몬스터 소환
    SpawnMonsters();

    // (옵션) 필요에 따라 상태 전환: BossRef->SetState(EBossState::Skill3);
    // Behavior Tree 내에서 후속 상태로 전환하도록 할 수 있으므로, 여기서는 단순히 태스크를 완료합니다.
    if (CachedOwnerComp)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}
