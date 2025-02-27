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
        }
    }

    PerformAOEAttack();
}

bool UBTTask_BossSkill3::CheckWallBlocking(UWorld* World, FVector BossLocation, FVector HitLocation, AActor* Wall, FCollisionQueryParams QueryParams)
{
    if (!Wall)
    {
        UE_LOG(LogTemp, Warning, TEXT("Wall is NULL"));
        return false;
    }

    if (!BossRef)
    {
        UE_LOG(LogTemp, Error, TEXT("BossRef is NULL in CheckWallBlocking"));
        return false;
    }

    BossLocation.Z += 50.0f;
    HitLocation.Z += 50.0f;

    QueryParams.AddIgnoredActor(BossRef);

    UE_LOG(LogTemp, Warning, TEXT("Executing SphereTrace from Boss to Player"));

    TArray<FHitResult> HitResults;
    bool bWallBlocking = World->SweepMultiByChannel(
        HitResults,
        BossLocation,
        HitLocation,
        FQuat::Identity,
        ECC_WorldStatic,
        FCollisionShape::MakeSphere(50.0f),
        QueryParams
    );

    DrawDebugSphere(World, BossLocation, 50.0f, 16, FColor::Red, true, 5.0f);

    if (bWallBlocking)
    {
        for (const FHitResult& WallHit : HitResults)
        {
            AActor* HitWallActor = WallHit.GetActor();

            if (!HitWallActor)
            {
                UE_LOG(LogTemp, Warning, TEXT("Wall Hit Actor is NULL"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Wall Detected: %s"), *HitWallActor->GetName());
                if (HitWallActor == Wall)
                {
                    return true;
                }
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("SphereTrace did not hit any walls"));

    return false;
}




void UBTTask_BossSkill3::PerformAOEAttack()
{
    if (!BossRef || bHasAttacked) return; // 이미 공격했으면 실행 안 함

    bHasAttacked = true; // 첫 실행 후 다시 실행되지 않도록 설정

    UWorld* World = BossRef->GetWorld();
    if (!World) return;

    FVector BossLocation = BossRef->GetActorLocation();
    float AttackRadius = BossRef->Skill3AttackRadius;

    TArray<AActor*> FoundWalls;
    TArray<FHitResult> HitResults;
    FCollisionQueryParams QueryParams;

    QueryParams.AddIgnoredActor(BossRef);

    if (BossRef->GetRootComponent())
    {
        QueryParams.AddIgnoredComponent(Cast<UPrimitiveComponent>(BossRef->GetRootComponent()));
    }

    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (PlayerPawn && PlayerPawn->GetRootComponent())
    {
        QueryParams.AddIgnoredComponent(Cast<UPrimitiveComponent>(PlayerPawn->GetRootComponent()));
    }

    UGameplayStatics::GetAllActorsOfClass(World, ABoss_Skill3_Wall::StaticClass(), FoundWalls);

    bool bHit = World->SweepMultiByChannel(
        HitResults,
        BossLocation,
        BossLocation,
        FQuat::Identity,
        ECC_WorldStatic,
        FCollisionShape::MakeSphere(AttackRadius),
        QueryParams
    );

    DrawDebugSphere(World, BossLocation, AttackRadius, 32, FColor::Blue, true, 5.0f);
    UE_LOG(LogTemp, Warning, TEXT("Detected %d Actors in AOE Range"), HitResults.Num());

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor) continue;

        UE_LOG(LogTemp, Warning, TEXT("Detected Actor: %s"), *HitActor->GetName());

        if (HitActor == BossRef)
        {
            UE_LOG(LogTemp, Warning, TEXT("Ignoring Boss: %s"), *HitActor->GetName());
            continue;
        }

        FVector HitLocation = HitActor->GetActorLocation();
        bool bBlocked = false;

        for (AActor* WallActor : FoundWalls)
        {
            if (CheckWallBlocking(World, BossLocation, HitLocation, WallActor, QueryParams))
            {
                bBlocked = true;
                break;
            }
        }

        DrawDebugString(World, HitLocation, bBlocked ? TEXT("Player (Blocked)") : TEXT("Player (Detected)"), nullptr, bBlocked ? FColor::Yellow : FColor::Green, 2.0f);
    }

    if (CachedOwnerComp)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}

