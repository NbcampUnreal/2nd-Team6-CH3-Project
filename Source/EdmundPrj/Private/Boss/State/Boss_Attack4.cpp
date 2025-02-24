#include "Boss/State/Boss_Attack4.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Boss/Boss_AnimInstance.h"
#include "Boss/Attack/Boss_Attack4_Bullet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

UBoss_Attack4::UBoss_Attack4()
{
    CurrentPhase = 0;
}

void UBoss_Attack4::EnterState(ABoss* Boss)
{
    Super::EnterState(Boss);
    UE_LOG(LogTemp, Log, TEXT("Boss is entering ATTACK4"));

    if (!Boss) return;
    BossRef = Boss;
    CurrentPhase = 0;
    BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Phase, [this]()
        {
            StartRise();
        }, WaitBeforeRise, false);
}

void UBoss_Attack4::StartRise()
{
    PlayAttack4Montage();
    if (BossRef && BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
    }

    CurrentPhase = 1;
}

void UBoss_Attack4::UpdateRise(float DeltaTime)
{
    if (!BossRef) return;
    FVector CurrentLocation = BossRef->GetActorLocation();
    FVector NewLocation = CurrentLocation + FVector(0, 0, Attack4_RiseSpeed * DeltaTime);
    if (NewLocation.Z >= Attack4_TargetHeight)
    {
        NewLocation.Z = Attack4_TargetHeight;
        BossRef->SetActorLocation(NewLocation, false);
        OnRiseComplete();
    }
    else
    {
        BossRef->SetActorLocation(NewLocation, false);
    }
}

void UBoss_Attack4::OnRiseComplete()
{
    CurrentPhase = 2;
    BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Phase, [this]()
        {
            StartBulletFire();
        }, WaitAtTarget, false);
}

void UBoss_Attack4::StartBulletFire()
{
    CurrentPhase = 3;
    BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_BulletFire, this, &UBoss_Attack4::ExecuteBulletFire, FireInterval, true);
}

void UBoss_Attack4::ExecuteBulletFire()
{
    if (!BossRef) return;

    static int32 FiredCount = 0;
    if (FiredCount >= FireBulletCount)
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(TimerHandle_BulletFire);
        FiredCount = 0;
        EndBulletFire();
        return;
    }

    FVector SpawnLocation = BossRef->MuzzleLocation->GetComponentLocation();
    AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
    if (Player)
    {
        FVector PlayerLocation = Player->GetActorLocation();
        FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();
        FRotator TargetRotation = Direction.Rotation();

        ABoss_Attack4_Bullet* Bullet = ABoss_Attack4_Bullet::GetBulletFromPool(BossRef->GetWorld(), BossRef->Attack4BulletClass);
        if (Bullet)
        {
            Bullet->FireProjectile(SpawnLocation, TargetRotation, Direction);
        }
    }
    FiredCount++;
}

void UBoss_Attack4::EndBulletFire()
{
    CurrentPhase = 4;
    StartDescend();
}

void UBoss_Attack4::StartDescend()
{
    CurrentPhase = 4;
}

void UBoss_Attack4::UpdateDescend(float DeltaTime)
{
    if (!BossRef) return;
    FVector CurrentLocation = BossRef->GetActorLocation();
    FVector NewLocation = CurrentLocation - FVector(0, 0, Attack4_DescendSpeed * DeltaTime);
    if (NewLocation.Z <= Attack4_GroundZ)
    {
        NewLocation.Z = Attack4_GroundZ;
        BossRef->SetActorLocation(NewLocation, false);
        OnDescendComplete();
    }
    else
    {
        BossRef->SetActorLocation(NewLocation, false);
    }
}

void UBoss_Attack4::OnDescendComplete()
{
    CurrentPhase = 5;
    BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Phase, this, &UBoss_Attack4::DelayedTransition, WaitAfterLanding, false);
}

void UBoss_Attack4::DelayedTransition()
{
    if (!BossRef) return;
    BossRef->SetState(EBossState::Idle);
}

void UBoss_Attack4::TickState(float DeltaTime)
{
    if (!BossRef) return;
    if (CurrentPhase == 1)
    {
        UpdateRise(DeltaTime);
    }
    else if (CurrentPhase == 4)
    {
        UpdateDescend(DeltaTime);
    }
}

void UBoss_Attack4::ExitState()
{
    if (BossRef)
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Phase);
        BossRef->GetWorld()->GetTimerManager().ClearTimer(TimerHandle_BulletFire);
        BossRef->GetWorld()->GetTimerManager().ClearTimer(TransitionTimerHandle);
    }
    BossRef = nullptr;
}

void UBoss_Attack4::PlayAttack4Montage()
{
    if (!BossRef) return;

    UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
    if (AnimInst && AnimInst->Attack4Montage)
    {
        float Duration = BossRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimInst->Attack4Montage);
    }
}
