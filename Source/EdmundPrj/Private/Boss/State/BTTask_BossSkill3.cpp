#include "Boss/State/BTTask_BossSkill3.h"
#include "Boss/Boss.h"
#include "Boss/Boss_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "AIController.h"
#include "TimerManager.h"

UBTTask_BossSkill3::UBTTask_BossSkill3()
{
    NodeName = TEXT("Boss Skill3");
    bNotifyTick = false;
    BossRef = nullptr;
    CachedOwnerComp = nullptr;
    SpawnedWallCount = 0;
}

EBTNodeResult::Type UBTTask_BossSkill3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AI 컨트롤러에서 Boss(Pawn)를 가져옵니다.
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

    // 정지: 보스의 이동을 즉시 멈춥니다.
    if (BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->StopMovementImmediately();
    }

    // 스킬 애니메이션 재생
    PlaySkillAnimation();

    // 벽 소환 개수 초기화
    SpawnedWallCount = 0;

    // 1초 후부터 Skill3SpawnInterval 간격으로 SpawnWall()을 반복 호출합니다.
    UWorld* World = BossRef->GetWorld();
    if (World)
    {
        World->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UBTTask_BossSkill3::SpawnWall, BossRef->Skill3SpawnInterval, true, 1.0f);
    }
    else
    {
        return EBTNodeResult::Failed;
    }

    // 태스크는 InProgress 상태로 유지됩니다.
    return EBTNodeResult::InProgress;
}

void UBTTask_BossSkill3::PlaySkillAnimation()
{
    if (!BossRef)
        return;

    USkeletalMeshComponent* Mesh = BossRef->GetMesh();
    if (!Mesh)
        return;

    UAnimInstance* AnimInst = Mesh->GetAnimInstance();
    if (!AnimInst)
        return;

    UBoss_AnimInstance* BossAnimInst = Cast<UBoss_AnimInstance>(AnimInst);
    if (BossAnimInst && BossAnimInst->Skill3Montage)
    {
        if (AnimInst->Montage_IsPlaying(BossAnimInst->Skill3Montage))
        {
            AnimInst->Montage_Stop(0.1f, BossAnimInst->Skill3Montage);
        }
        AnimInst->Montage_Play(BossAnimInst->Skill3Montage);
    }
}

void UBTTask_BossSkill3::SpawnWall()
{
    if (!BossRef)
        return;

    // Skill3WallClass가 지정되어 있지 않으면 아무것도 하지 않음
    if (BossRef->Skill3WallClass == nullptr)
    {
        return;
    }

    UWorld* World = BossRef->GetWorld();
    if (!World)
        return;

    // 소환 위치 계산:
    // - BossRef->Skill3MinSpawnDistance와 BossRef->Skill3SpawnRadius 사이의 랜덤 거리
    // - 보스의 위치에서 랜덤한 XY 방향 (Z는 0)으로 이동
    float Distance = FMath::RandRange(BossRef->Skill3MinSpawnDistance, BossRef->Skill3SpawnRadius);
    FVector RandomDirection = FMath::VRand();
    RandomDirection.Z = 0.f;
    RandomDirection.Normalize();
    FVector SpawnLocation = BossRef->GetActorLocation() + RandomDirection * Distance;
    // Z 값은 2000 유닛 만큼 올려서 소환 (예: 하늘 위)
    SpawnLocation.Z += 2000.0f;

    // 소환 회전 계산
    FRotator SpawnRotation = CalculateRandomRotation();

    // 벽(또는 몬스터) 소환
    AActor* SpawnedWall = World->SpawnActor<AActor>(BossRef->Skill3WallClass, SpawnLocation, SpawnRotation);

    SpawnedWallCount++;
    // 소환한 벽의 개수가 설정된 Skill3WallCount 이상이면 스폰 완료 처리
    if (SpawnedWallCount >= BossRef->Skill3WallCount)
    {
        OnSpawnComplete();
    }
}

FRotator UBTTask_BossSkill3::CalculateRandomRotation()
{
    float Pitch = FMath::RandRange(-30.0f, 30.0f);
    float Yaw = FMath::RandRange(-30.0f, 30.0f);
    float Roll = FMath::RandRange(-30.0f, 30.0f);
    return FRotator(Pitch, Yaw, Roll);
}

void UBTTask_BossSkill3::OnSpawnComplete()
{
    if (BossRef)
    {
        UWorld* World = BossRef->GetWorld();
        if (World)
        {
            World->GetTimerManager().ClearTimer(SpawnTimerHandle);
        }
        // 상태 전환은 Behavior Tree에서 처리하거나 여기서 직접 호출할 수 있습니다.
        // 예를 들어, BossRef->SetState(EBossState::Idle); 와 같이 할 수 있습니다.
    }
    if (CachedOwnerComp)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}
