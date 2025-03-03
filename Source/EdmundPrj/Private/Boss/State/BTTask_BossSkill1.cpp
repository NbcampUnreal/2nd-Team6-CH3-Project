#include "Boss/State/BTTask_BossSkill1.h"
#include "Boss/Boss.h"
#include "AIController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

#include "Engine/OverlapResult.h"
#include "Engine/EngineTypes.h"
#include "WorldCollision.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

#include "Boss/Boss_AnimInstance.h"

UBTTask_BossSkill1::UBTTask_BossSkill1()
{
    NodeName = TEXT("BossSkill1");
    bNotifyTick = false;

    BossRef = nullptr;
    CachedOwnerComp = nullptr;
    CurrentOverlapCount = 0;
}

EBTNodeResult::Type UBTTask_BossSkill1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    if (BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->StopMovementImmediately();
    }

    PlaySkill1Animation();
    if (BossRef->Skill1UpperEffect)
    {
        FVector UpperEffectLocation = BossRef->GetActorLocation() + FVector(0.f, 0.f, 200.f);
        UGameplayStatics::SpawnEmitterAtLocation(BossRef->GetWorld(), BossRef->Skill1UpperEffect, UpperEffectLocation);
    }
    if (BossRef->Skill1LowerEffect)
    {
        FVector LowerEffectLocation = BossRef->GetActorLocation() + FVector(0.f, 0.f, -50.f);
        UGameplayStatics::SpawnEmitterAtLocation(BossRef->GetWorld(), BossRef->Skill1LowerEffect, LowerEffectLocation);
    }

    CurrentOverlapCount = 0;
    if (BossRef->GetWorld())
    {
        BossRef->GetWorldTimerManager().SetTimer(
            StartWaitTimerHandle,
            this,
            &UBTTask_BossSkill1::OnStartWaitComplete,
            3.0f,
            false
        );
    }

    return EBTNodeResult::InProgress;
}

void UBTTask_BossSkill1::PlaySkill1Animation()
{
    if (!BossRef) return;

    // 1) Mesh / AnimInst
    USkeletalMeshComponent* Mesh = BossRef->GetMesh();
    if (!Mesh) return;

    UAnimInstance* AnimInst = Mesh->GetAnimInstance();
    if (!AnimInst) return;

    UBoss_AnimInstance* BossAnimInst = Cast<UBoss_AnimInstance>(AnimInst);
    if (BossAnimInst && BossAnimInst->Skill1Montage)
    {
        if (AnimInst->Montage_IsPlaying(BossAnimInst->Skill1Montage))
        {
            AnimInst->Montage_Stop(0.1f, BossAnimInst->Skill1Montage);
        }
        AnimInst->Montage_Play(BossAnimInst->Skill1Montage);
    }
}

void UBTTask_BossSkill1::OnStartWaitComplete()
{
    StartOverlap();
}

void UBTTask_BossSkill1::StartOverlap()
{
    bool bFloorPattern = (FMath::RandRange(0, 1) == 0);

    PerformOverlapCheck(bFloorPattern);

    if (BossRef && BossRef->GetWorld())
    {
        BossRef->GetWorldTimerManager().SetTimer(
            OverlapTimerHandle,
            this,
            &UBTTask_BossSkill1::OnOverlapEnd,
            0.5f,
            false
        );
    }
}

void UBTTask_BossSkill1::PerformOverlapCheck(bool bFloorPattern)
{
    if (!BossRef) return;
    UWorld* World = BossRef->GetWorld();
    if (!World) return;

    FVector Center = BossRef->GetActorLocation();

    if (bFloorPattern)
    {
        Center.Z = 0.f;
    }
    else
    {
        ACharacter* PlayerChar = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(World, 0));
        if (PlayerChar && PlayerChar->GetMesh())
        {
            FVector FootPos = PlayerChar->GetMesh()->GetSocketLocation(TEXT("CharacterFoot"));
            FVector HeadPos = PlayerChar->GetMesh()->GetSocketLocation(TEXT("CharacterHead"));
            float DynamicOffset = (HeadPos.Z - FootPos.Z) * 0.5f;
            Center.Z = FootPos.Z + DynamicOffset;
        }
        else
        {
            Center.Z += 110.f;
        }
    }

    FVector BoxExtent(5000.f, 5000.f, 10.f);

    TArray<FOverlapResult> OverlapResults;
    FCollisionShape Shape = FCollisionShape::MakeBox(BoxExtent);

    bool bOverlapped = World->OverlapMultiByChannel(
        OverlapResults,
        Center,
        FQuat::Identity,
        ECC_Pawn,
        Shape
    );

    DrawDebugBox(World, Center, BoxExtent, (bFloorPattern ? FColor::Blue : FColor::Magenta), false, 0.5f);

    if (bOverlapped)
    {
        for (auto& Over : OverlapResults)
        {
            if (AActor* OverActor = Over.GetActor())
            {
                if (ACharacter* OverChar = Cast<ACharacter>(OverActor))
                {
                    if (OverChar != BossRef)
                    {
                        float DamageValue = BossRef->GetMonsterAttackDamage() * BossRef->GetSkill1Multiplier();
                        UGameplayStatics::ApplyDamage(
                            OverChar,
                            DamageValue,
                            nullptr,
                            BossRef,
                            UDamageType::StaticClass()
                        );
                    }
                }
            }
        }
    }
}


void UBTTask_BossSkill1::OnOverlapEnd()
{
    if (BossRef && BossRef->GetWorld())
    {
        BossRef->GetWorldTimerManager().SetTimer(
            GapTimerHandle,
            this,
            &UBTTask_BossSkill1::OnGapEnd,
            1.0f,
            false
        );
    }
}

void UBTTask_BossSkill1::OnGapEnd()
{
    CurrentOverlapCount++;
    PlaySkill1Animation();

    if (CurrentOverlapCount < 5)
    {
        StartOverlap();
    }
    else
    {
        EndTask();
    }
}

void UBTTask_BossSkill1::EndTask()
{
    if (BossRef)
    {
        BossRef->SetbSkill1Used(true);
    }
    if (CachedOwnerComp)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}