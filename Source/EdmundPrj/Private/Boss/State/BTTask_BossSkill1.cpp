#include "Boss/State/BTTask_BossSkill1.h"
#include "Boss/Boss.h"
#include "AIController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
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
    AttackIterationCount = 0;
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

    PlayInitialMontage();

    if (BossRef->GetWorld())
    {
        BossRef->GetWorldTimerManager().SetTimer(InitialMontageTimerHandle, this, &UBTTask_BossSkill1::StartAttackPattern, 1.0f, false);
    }

    return EBTNodeResult::InProgress;
}

void UBTTask_BossSkill1::PlayInitialMontage()
{
    if (!BossRef) return;

    USkeletalMeshComponent* Mesh = BossRef->GetMesh();
    if (!Mesh) return;

    UAnimInstance* AnimInst = Mesh->GetAnimInstance();
    if (!AnimInst) return;

    UBoss_AnimInstance* BossAnimInst = Cast<UBoss_AnimInstance>(AnimInst);
    if (BossAnimInst && BossAnimInst->Skill1_1Montage)
    {
        if (AnimInst->Montage_IsPlaying(BossAnimInst->Skill1_1Montage))
        {
            AnimInst->Montage_Stop(0.1f, BossAnimInst->Skill1_1Montage);
        }
        AnimInst->Montage_Play(BossAnimInst->Skill1_1Montage);
    }
}


void UBTTask_BossSkill1::StartAttackPattern()
{
    AttackIterationCount = 0;
    PerformAttackIteration();
}

void UBTTask_BossSkill1::PerformAttackIteration()
{
    if (!BossRef) return;

    bool bIsHeadAttack = (FMath::RandRange(0, 1) == 0);

    USkeletalMeshComponent* Mesh = BossRef->GetMesh();
    if (!Mesh) return;

    UAnimInstance* AnimInst = Mesh->GetAnimInstance();
    if (!AnimInst) return;

    UBoss_AnimInstance* BossAnimInst = Cast<UBoss_AnimInstance>(AnimInst);
    if (!BossAnimInst) return;

    UAnimMontage* HintMontage = bIsHeadAttack ? BossAnimInst->Skill1_2Montage : BossAnimInst->Skill1_3Montage;
    if (HintMontage)
    {
        float MontageDuration = AnimInst->Montage_Play(HintMontage);
        FTimerHandle TimerHandle;
        BossRef->GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UBTTask_BossSkill1::OnAttackHintFinished, bIsHeadAttack), MontageDuration, false);
    }
    else
    {
        OnAttackHintFinished(bIsHeadAttack);
    }
}


void UBTTask_BossSkill1::OnAttackHintFinished(bool bIsHeadAttack)
{
    bool bFloorPattern = !bIsHeadAttack;
    PerformOverlapCheck(bFloorPattern);

    if (BossRef && BossRef->GetWorld())
    {
        BossRef->GetWorldTimerManager().SetTimer(AttackIterationTimerHandle, this, &UBTTask_BossSkill1::OnAttackIterationEnd, 1.0f, false);
    }
}

void UBTTask_BossSkill1::OnAttackIterationEnd()
{
    AttackIterationCount++;
    if (AttackIterationCount < 5)
    {
        PerformAttackIteration();
    }
    else
    {
        EndTask();
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
        if (BossRef->Skill1LowerEffect)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                World,
                BossRef->Skill1LowerEffect,
                Center,
                FRotator::ZeroRotator
            );
        }
    }
    else
    {
        ACharacter* PlayerChar = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(World, 0));
        if (PlayerChar && PlayerChar->GetMesh())
        {
            FVector FootPos = PlayerChar->GetMesh()->GetSocketLocation(TEXT("CharacterFoot"));
            FVector HeadPos = PlayerChar->GetMesh()->GetSocketLocation(TEXT("CharacterHead"));
            float DynamicOffset = (HeadPos.Z - FootPos.Z) * 0.5f;
            Center.Z = FootPos.Z + DynamicOffset + 30.0f;
        }
        else
        {
            Center.Z += 110.f;
        }
        if (BossRef->Skill1UpperEffect)
        {
            FVector EffectLocation = Center;
            EffectLocation.Z += 500.f;
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                World,
                BossRef->Skill1UpperEffect,
                EffectLocation,
                FRotator::ZeroRotator
            );
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

    //DrawDebugBox(World, Center, BoxExtent, (bFloorPattern ? FColor::Blue : FColor::Magenta), false, 0.5f);

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
