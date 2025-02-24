#include "Boss/State/Boss_Skill3.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Boss/Boss_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

UBoss_Skill3::UBoss_Skill3()
{
}

void UBoss_Skill3::EnterState(ABoss* Boss)
{
    Super::EnterState(Boss);
    UE_LOG(LogTemp, Log, TEXT("Skill3 start"));

    BossRef = Boss;
    if (!BossRef)
    {
        return;
    }

    if (BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->StopMovementImmediately();
    }

    PlaySkillAnimation();
    UWorld* World = BossRef->GetWorld();
    if (World)
    {
        SpawnedWallCount = 0;
        World->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UBoss_Skill3::SpawnWall, BossRef->Skill3SpawnInterval, true, 1.0f);
    }
}

void UBoss_Skill3::PlaySkillAnimation()
{
    if (!BossRef)
        return;

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
    if (BossAnimInst && BossAnimInst->Skill3Montage)
    {
        if (AnimInst->Montage_IsPlaying(BossAnimInst->Skill3Montage))
        {
            AnimInst->Montage_Stop(0.1f, BossAnimInst->Skill3Montage);
        }
        float Duration = AnimInst->Montage_Play(BossAnimInst->Skill3Montage);
    }
    
}

void UBoss_Skill3::SpawnWall()
{
    if (!BossRef)
        return;

    if (BossRef->Skill3WallClass == nullptr)
    {
        return;
    }

    UWorld* World = BossRef->GetWorld();
    if (!World)
    {
        return;
    }

    float Distance = FMath::RandRange(BossRef->Skill3MinSpawnDistance, BossRef->Skill3SpawnRadius);
    FVector RandomDirection = FMath::VRand();
    RandomDirection.Z = 0.f;
    RandomDirection.Normalize();
    FVector SpawnLocation = BossRef->GetActorLocation() + RandomDirection * Distance;
    FRotator SpawnRotation = CalculateRandomRotation();
    SpawnLocation.Z += 2000.0f;
    AActor* SpawnedWall = World->SpawnActor<AActor>(BossRef->Skill3WallClass, SpawnLocation, SpawnRotation);

    SpawnedWallCount++;
    if (SpawnedWallCount >= BossRef->Skill3WallCount)
    {
        OnSpawnComplete();
    }
}

void UBoss_Skill3::OnSpawnComplete()
{
    if (BossRef)
    {
        UWorld* World = BossRef->GetWorld();
        if (World)
        {
            World->GetTimerManager().ClearTimer(SpawnTimerHandle);
        }
        BossRef->SetState(EBossState::Idle);
    }
}

void UBoss_Skill3::TickState(float DeltaTime)
{
}

void UBoss_Skill3::ExitState()
{
    UE_LOG(LogTemp, Log, TEXT("Skill3 end"));
    if (BossRef)
    {
        UWorld* World = BossRef->GetWorld();
        if (World)
        {
            World->GetTimerManager().ClearTimer(SpawnTimerHandle);
        }
    }
    BossRef = nullptr;
}

FRotator UBoss_Skill3::CalculateRandomRotation()
{
    float Pitch = FMath::RandRange(-30.0f, 30.0f);
    float Yaw = FMath::RandRange(-30.0f, 30.0f);
    float Roll = FMath::RandRange(-30.0f, 30.0f);
    return FRotator(Pitch, Yaw, Roll);
}