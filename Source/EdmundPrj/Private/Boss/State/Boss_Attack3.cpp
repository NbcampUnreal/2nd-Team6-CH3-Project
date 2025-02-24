#include "Boss/State/Boss_Attack3.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Boss/Boss_AnimInstance.h"

UBoss_Attack3::UBoss_Attack3()
{
    ComboPhase = 1;
    RangedComboBulletCount = 0;
}

void UBoss_Attack3::EnterState(ABoss* Boss)
{
    Super::EnterState(Boss);
    UE_LOG(LogTemp, Log, TEXT("Boss is entering ATTACK3"));

    if (!Boss) return;
    BossRef = Boss;
    ComboPhase = 1;
    RangedComboBulletCount = 0;

    BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_ComboPhase, this, &UBoss_Attack3::ExecuteMeleeAttack, 3.0f, false);
}

void UBoss_Attack3::ExecuteMeleeAttack()
{
    if (!BossRef) return;

    float DashDistance = (ComboPhase == 1) ? 200.0f : 300.0f;
    FVector DashDirection = BossRef->GetActorForwardVector();
    FVector CurrentLocation = BossRef->GetActorLocation();
    FVector NewLocation = CurrentLocation + DashDirection * DashDistance;
    BossRef->SetActorLocation(NewLocation, true);

    BossRef->MonsterAttackCheck();
    if (ComboPhase == 1)
    {
        PlayAttack3Montage();
    }
    ComboPhase++;
    if (ComboPhase <= 2)
    {
        BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_ComboPhase, this, &UBoss_Attack3::ExecuteMeleeAttack, 0.5f, false);
    }
    else if (ComboPhase == 3)
    {
        ExecuteRangedComboAttack();
    }
}

void UBoss_Attack3::ExecuteRangedComboAttack()
{
    if (!BossRef) return;
    RangedComboBulletCount = 0;

    BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_RangedCombo, [this]()
        {
            if (BossRef && BossRef->Attack1BulletClass)
            {
                FVector SpawnLocation = BossRef->MuzzleLocation->GetComponentLocation();
                AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
                if (Player)
                {
                    FVector PlayerLocation = Player->GetActorLocation();
                    FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();
                    FRotator TargetRotation = Direction.Rotation();

                    ABoss_Attack1_Bullet* Bullet = ABoss_Attack1_Bullet::GetBulletFromPool(BossRef->GetWorld(), BossRef->Attack1BulletClass);
                    if (Bullet)
                    {
                        Bullet->FireProjectile(SpawnLocation, TargetRotation, Direction);
                    }
                }
            }
            RangedComboBulletCount++;
            if (RangedComboBulletCount >= 5)
            {
                BossRef->GetWorld()->GetTimerManager().ClearTimer(TimerHandle_RangedCombo);
                EndRangedComboAttack();
            }
        }, 0.2f, true);
}


void UBoss_Attack3::EndRangedComboAttack()
{
    if (BossRef)
    {
        BossRef->SetState(EBossState::Idle);
    }
}

void UBoss_Attack3::TickState(float DeltaTime)
{
}

void UBoss_Attack3::ExitState()
{
    if (BossRef)
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ComboPhase);
        BossRef->GetWorld()->GetTimerManager().ClearTimer(TimerHandle_RangedCombo);
    }
    BossRef = nullptr;
}

void UBoss_Attack3::PlayAttack3Montage()
{
    if (!BossRef) return;

    UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
    if (AnimInst && AnimInst->Attack3Montage)
    {
        float Duration = BossRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimInst->Attack3Montage);
        BossRef->GetWorld()->GetTimerManager().SetTimer(TransitionTimerHandle, this, &UBoss_Attack3::DelayedTransition, Duration, false);
    }
}

void UBoss_Attack3::DelayedTransition()
{
    if (!BossRef) return;
    BossRef->SetState(EBossState::Idle);
}
