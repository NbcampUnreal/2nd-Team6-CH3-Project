#include "Boss/State/BTTask_BossAttack4.h"
#include "Boss/Boss.h"
#include "AIController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/Attack/Boss_Attack4_Bullet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Boss/Boss_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_BossAttack4::UBTTask_BossAttack4()
{
    NodeName = TEXT("Boss Attack4");
    bNotifyTick = true;
    CurrentPhase = 0;
    BossRef = nullptr;
    CachedOwnerComp = nullptr;
}

EBTNodeResult::Type UBTTask_BossAttack4::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }
    BossRef = Cast<ABoss>(AIController->GetPawn());
    if (!BossRef)
    {
        return EBTNodeResult::Failed;
    }

    CachedOwnerComp = &OwnerComp;
    BossRef->Attack4_Attack4FiredCount = 0;
    CurrentPhase = 0;

    BossRef->GetWorld()->GetTimerManager().SetTimer(
        TimerHandle_Phase, this, &UBTTask_BossAttack4::StartRise, BossRef->Attack4_WaitBeforeRise, false);

    return EBTNodeResult::InProgress;
}

void UBTTask_BossAttack4::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (!BossRef)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // 상승
    if (CurrentPhase == 1)
    {
        UpdateRise(DeltaSeconds);
    }
    // 하강
    else if (CurrentPhase == 4)
    {
        UpdateDescend(DeltaSeconds);
    }
}

//****************상승*************************
void UBTTask_BossAttack4::StartRise()
{
    if (!BossRef) return;

    UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
    if (AnimInst && AnimInst->Attack4Montage)
    {
        float Duration = BossRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimInst->Attack4Montage);
    }

    if (BossRef->GetCharacterMovement())
    {
        BossRef->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        BossRef->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
    }

    CurrentPhase = 1;
}

void UBTTask_BossAttack4::UpdateRise(float DeltaTime)
{
    if (!BossRef) return;

    FVector CurrentLocation = BossRef->GetActorLocation();
    FVector NewLocation = CurrentLocation + FVector(0, 0, BossRef->Attack4_RiseSpeed * DeltaTime);

    if (NewLocation.Z >= BossRef->Attack4_TargetHeight)
    {
        NewLocation.Z = BossRef->Attack4_TargetHeight;
        BossRef->SetActorLocation(NewLocation, false);
        OnRiseComplete();
    }
    else
    {
        BossRef->SetActorLocation(NewLocation, false);
    }
}

void UBTTask_BossAttack4::OnRiseComplete()
{
    CurrentPhase = 2;
    BossRef->GetWorld()->GetTimerManager().SetTimer(
        TimerHandle_Phase, this, &UBTTask_BossAttack4::StartBulletFire, BossRef->Attack4_WaitAtTarget, false);
}

// ****************탄 관련*******************
void UBTTask_BossAttack4::StartBulletFire()
{
    CurrentPhase = 3;
    BossRef->GetWorld()->GetTimerManager().SetTimer(
        TimerHandle_BulletFire, this, &UBTTask_BossAttack4::ExecuteBulletFire, BossRef->Attack4_FireInterval, true);
}

void UBTTask_BossAttack4::ExecuteBulletFire()
{
    if (!BossRef) return;

    if (BossRef->Attack4_Attack4FiredCount >= BossRef->Attack4_FireBulletCount)
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(TimerHandle_BulletFire);
        BossRef->Attack4_Attack4FiredCount = 0;
        EndBulletFire();
        return;
    }

    if (!BossRef->MuzzleLocation)
    {
        return;
    }

    FVector SpawnLocation = BossRef->MuzzleLocation->GetComponentLocation();
    AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
    if (!Player)
    {
        return;
    }

    FVector PlayerLocation = Player->GetActorLocation();
    FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();
    FRotator TargetRotation = Direction.Rotation();

    ABoss_Attack4_Bullet* Bullet = ABoss_Attack4_Bullet::GetBulletFromPool(BossRef->GetWorld(), BossRef->Attack4BulletClass);
    if (Bullet)
    {
        if (BossRef->GameState)
        {
            BossRef->GameState->PlayMonsterSound(BossRef->CurrentAudioComp, BossRef->GetMonsterType(), ESoundType::Attack4);
        }
        Bullet->FireProjectile(SpawnLocation, TargetRotation, Direction);
    }

    BossRef->Attack4_Attack4FiredCount++;
}

void UBTTask_BossAttack4::EndBulletFire()
{
    CurrentPhase = 4;
}

//*****************하강*************************
void UBTTask_BossAttack4::UpdateDescend(float DeltaTime)
{
    if (!BossRef)
    {
        return;
    }

    FVector CurrentLocation = BossRef->GetActorLocation();
    FVector NewLocation = CurrentLocation - FVector(0, 0, BossRef->Attack4_DescendSpeed * DeltaTime);
    FHitResult HitResult;
    FVector TraceStart = CurrentLocation;
    FVector TraceEnd = TraceStart - FVector(0, 0, 5000.0f);
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(BossRef);

    if (BossRef->GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
    {
        float GroundZ = HitResult.Location.Z;
        float CapsuleOffset = BossRef->Front_Left_FootCapsuleComponent ? BossRef->Front_Left_FootCapsuleComponent->GetScaledCapsuleHalfHeight() : 300.0f;
        if (NewLocation.Z <= GroundZ + CapsuleOffset)
        {
            NewLocation.Z = GroundZ + CapsuleOffset;
            BossRef->SetActorLocation(NewLocation, false);
            FRotator CurrentRotation = BossRef->GetActorRotation();
            BossRef->SetActorRotation(CurrentRotation);

            if (BossRef->GetCharacterMovement())
            {
                BossRef->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                BossRef->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
            }
            BossRef->UpdateAttackCooldown(4);
            FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
            return;
        }
    }
    BossRef->SetActorLocation(NewLocation, false);
}




//void UBTTask_BossAttack4::OnDescendComplete()
//{
//    CurrentPhase = 5;
//    BossRef->GetWorld()->GetTimerManager().SetTimer(
//        TimerHandle_Transition, this, &UBTTask_BossAttack4::DelayedTransition, BossRef->Attack4_WaitAfterLanding, false);
//}
//
//void UBTTask_BossAttack4::DelayedTransition()
//{
//    if (!BossRef || !CachedOwnerComp)
//    {
//        return;
//    }
//    BossRef->UpdateAttackCooldown(4);
//    FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
//}
