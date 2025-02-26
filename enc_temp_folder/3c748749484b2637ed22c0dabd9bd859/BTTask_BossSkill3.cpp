#include "Boss/State/BTTask_BossSkill3.h"
#include "Boss/Boss.h"
#include "Boss/Attack/Boss_Skill3_Wall.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Boss/Boss_AnimInstance.h"
#include "Kismet/GameplayStatics.h"

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
    // Boss 참조 가져오기
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

    // 보스 이동 정지
    if (BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->StopMovementImmediately();
    }

    // 스킬 애니메이션 재생
    PlaySkillAnimation();

    SpawnedWallCount = 0;
    UWorld* World = BossRef->GetWorld();
    if (World)
    {
        // 1초 후부터 Skill3SpawnInterval 간격으로 SpawnWall() 호출
        World->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UBTTask_BossSkill3::SpawnWall, BossRef->Skill3SpawnInterval, true, 1.0f);
    }
    else
    {
        return EBTNodeResult::Failed;
    }
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

    if (BossRef->Skill3WallClass == nullptr)
    {
        return;
    }

    UWorld* World = BossRef->GetWorld();
    if (!World)
        return;

    // 소환 위치 계산
    float Distance = FMath::RandRange(BossRef->Skill3MinSpawnDistance, BossRef->Skill3SpawnRadius);
    FVector RandomDirection = FMath::VRand();
    RandomDirection.Z = 0.f;
    RandomDirection.Normalize();
    FVector SpawnLocation = BossRef->GetActorLocation() + RandomDirection * Distance;
    SpawnLocation.Z += 2000.0f; // 위쪽에서 소환 후 중력 적용

    FRotator SpawnRotation = CalculateRandomRotation();

    // TSubclassOf<AActor>를 TSubclassOf<ABoss_Skill3_Wall>로 캐스팅
    TSubclassOf<ABoss_Skill3_Wall> WallClass = TSubclassOf<ABoss_Skill3_Wall>(BossRef->Skill3WallClass.Get());

    ABoss_Skill3_Wall* SpawnedWall = ABoss_Skill3_Wall::GetWallFromPool(World, WallClass);
    if (SpawnedWall)
    {
        SpawnedWall->ActivateWall(SpawnLocation, SpawnRotation);
    }

    SpawnedWallCount++;
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
        BossRef->SetState(EBossState::Chase);
    }
    if (CachedOwnerComp)
    {
        BossRef->SetState(EBossState::Chase);
        //FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}
