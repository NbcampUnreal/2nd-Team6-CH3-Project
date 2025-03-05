#include "Boss/State/BTTask_BossSkill3.h"
#include "Boss/Boss.h"
#include "Boss/Attack/Boss_Skill3_Wall.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Boss/Boss_AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

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
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return EBTNodeResult::Failed;
    BossRef = Cast<ABoss>(AICon->GetPawn());
    if (!BossRef) return EBTNodeResult::Failed;

    CachedOwnerComp = &OwnerComp;
    if (BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->StopMovementImmediately();
    }

    PlaySkillAnimation();
    BossRef->SetbSkill3Used(true);
    SpawnedWallCount = 0;

    UWorld* World = BossRef->GetWorld();
    if (World)
    {
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
    if (!BossRef) return;

    USkeletalMeshComponent* Mesh = BossRef->GetMesh();
    if (!Mesh) return;

    UAnimInstance* AnimInst = Mesh->GetAnimInstance();
    if (!AnimInst) return;

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
    if (!BossRef) return;
    if (BossRef->Skill3WallClass == nullptr) return;

    UWorld* World = BossRef->GetWorld();
    if (!World) return;

    float Distance = FMath::RandRange(BossRef->Skill3MinSpawnDistance, BossRef->Skill3SpawnRadius);
    FVector RandomDirection = FMath::VRand();
    RandomDirection.Z = 0.f;
    RandomDirection.Normalize();
    FVector SpawnLocation = BossRef->GetActorLocation() + RandomDirection * Distance;
    SpawnLocation.Z += 2000.0f;

    FRotator SpawnRotation = CalculateRandomRotation();
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
            World->GetTimerManager().SetTimer(DelayedStartTimer, this, &UBTTask_BossSkill3::StartDetection, BossRef->Skill3StartDelay, false);
        }
    }
}

void UBTTask_BossSkill3::StartDetection()
{
    if (!BossRef) return;

    UWorld* World = BossRef->GetWorld();
    if (!World) return;

    if (BossRef->Skill3Particle)
    {
        BossRef->Skill3Particle->Activate(true);
    }

    World->GetTimerManager().SetTimer(DetectionTimer, this, &UBTTask_BossSkill3::PerformDetection, 0.1f, true, 0.0f);
    World->GetTimerManager().SetTimer(EndTimer, this, &UBTTask_BossSkill3::StopDetection, 1.0f, false);
}

void UBTTask_BossSkill3::PerformDetection()
{
    if (!BossRef) return;

    UWorld* World = BossRef->GetWorld();
    if (!World) return;

    FVector BossLocation = BossRef->GetActorLocation();
    BossLocation.Z += 100.0f;

    float DetectionRadius = BossRef->Skill3DetectionRadius;

    TArray<FHitResult> HitResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(BossRef);

    bool bHit = World->SweepMultiByChannel(
        HitResults,
        BossLocation,
        BossLocation,
        FQuat::Identity,
        ECC_GameTraceChannel2,
        FCollisionShape::MakeSphere(DetectionRadius),
        QueryParams
    );

    bool bCharacterDetected = false;
    FString DetectedActorsLog = "";

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor) continue;
        if (!(HitActor->ActorHasTag("Player") || HitActor->ActorHasTag("Skill3Wall")))
        {
            continue;
        }

        if (HitActor->ActorHasTag("Player"))
        {
            FHitResult WallHit;
            bool bWallBlocking = World->LineTraceSingleByChannel(
                WallHit,
                BossLocation,
                HitActor->GetActorLocation(),
                ECC_WorldStatic,
                QueryParams
            );

            if (bWallBlocking && WallHit.GetActor() && WallHit.GetActor()->ActorHasTag("Skill3Wall"))
            {
                //UE_LOG(LogTemp, Warning, TEXT("[Wall Blocked]: %s"), *WallHit.GetActor()->GetName());
                continue;
            }

            bCharacterDetected = true;
            UGameplayStatics::ApplyDamage(HitActor, 999999.f, nullptr, BossRef, UDamageType::StaticClass());
            DetectedActorsLog += HitActor->GetName() + TEXT(" ");
        }
    }

    //UE_LOG(LogTemp, Warning, TEXT("HitResults Count: %d"), HitResults.Num());

    UE_LOG(LogTemp, Warning, TEXT("%s"), bCharacterDetected ? TEXT("Player Die") : TEXT("Hide"));

    DrawDebugSphere(World, BossLocation, DetectionRadius, 32, bCharacterDetected ? FColor::Red : FColor::Blue, false, 0.5f);
}

void UBTTask_BossSkill3::StopDetection()
{
    if (!BossRef) return;

    UWorld* World = BossRef->GetWorld();
    if (!World) return;

    World->GetTimerManager().ClearTimer(DetectionTimer);

    if (BossRef->Skill3Particle)
    {
        BossRef->Skill3Particle->Deactivate();
    }

    if (CachedOwnerComp)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}
