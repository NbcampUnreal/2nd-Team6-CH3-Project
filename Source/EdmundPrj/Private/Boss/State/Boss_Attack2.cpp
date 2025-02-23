#include "Boss/State/Boss_Attack2.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Boss/Boss_AnimInstance.h"

UBoss_Attack2::UBoss_Attack2()
{
    // 초기 단계: 0 (InitialWait)
    CurrentPhase = 0;
}

void UBoss_Attack2::EnterState(ABoss* Boss)
{
    Super::EnterState(Boss);
    UE_LOG(LogTemp, Log, TEXT("Boss is entering ATTACK2 state"));

    if (!Boss) return;
    BossRef = Boss;
    CurrentPhase = 0; // InitialWait

    BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Phase, [this]()
        {
            StartAscend();
        }, 2.0f, false);
}

void UBoss_Attack2::StartAscend()
{
    UE_LOG(LogTemp, Log, TEXT("Boss starting to ascend"));

    if (BossRef && BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
    }

    CurrentPhase = 1; // Ascending

    DelayedFire_Attack2();
}

void UBoss_Attack2::OnAscendComplete()
{
    UE_LOG(LogTemp, Log, TEXT("Boss reached target height, waiting 1 second at max height"));
    CurrentPhase = 2; // AtMaxWait

    BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Phase, [this]()
        {
            AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
            if (Player)
            {
                FVector PlayerLocation = Player->GetActorLocation();
                FVector CurrentLocation = BossRef->GetActorLocation();
                TargetHorizontalLocation = FVector(PlayerLocation.X, PlayerLocation.Y, CurrentLocation.Z);
            }
            else
            {
                TargetHorizontalLocation = BossRef->GetActorLocation();
            }
            UE_LOG(LogTemp, Log, TEXT("Boss starting horizontal reposition to (%f, %f)"),
                TargetHorizontalLocation.X, TargetHorizontalLocation.Y);
            CurrentPhase = 3; // HorizontalReposition

        }, 1.0f, false);
}

void UBoss_Attack2::StartDescend()
{
    UE_LOG(LogTemp, Log, TEXT("Boss starting to descend"));
    CurrentPhase = 5;
}

void UBoss_Attack2::TickState(float DeltaTime)
{
    if (!BossRef) return;

    switch (CurrentPhase)
    {
    case 1: // Ascending
    {
        FVector CurrentLocation = BossRef->GetActorLocation();
        FVector NewLocation = CurrentLocation + FVector(0, 0, BossRef->Attack2_AscendSpeed * DeltaTime);
        UE_LOG(LogTemp, Log, TEXT("Ascending: Current Z: %f, New Z: %f, Target Z: %f"),
            CurrentLocation.Z, NewLocation.Z, BossRef->Attack2_TargetHeight);

        if (NewLocation.Z >= BossRef->Attack2_TargetHeight)
        {
            NewLocation.Z = BossRef->Attack2_TargetHeight;
            BossRef->SetActorLocation(NewLocation, false);
            CurrentPhase = 6;
            OnAscendComplete();
        }
        else
        {
            BossRef->SetActorLocation(NewLocation, false);
        }
        break;
    }
    case 3:
    {
        FVector CurrentLocation = BossRef->GetActorLocation();
        FVector Direction = (TargetHorizontalLocation - CurrentLocation);
        Direction.Z = 0;
        float Distance = Direction.Size();

        if (Distance < 10.0f)
        {
            BossRef->SetActorLocation(FVector(TargetHorizontalLocation.X, TargetHorizontalLocation.Y, CurrentLocation.Z));
            CurrentPhase = 4;
            BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Phase, [this]()
                {
                    StartDescend();
                }, 1.0f, false);
        }
        else
        {
            Direction.Normalize();
            FVector NewLocation = CurrentLocation + Direction * BossRef->Attack2_HorizontalMoveSpeed * DeltaTime;
            NewLocation.Z = CurrentLocation.Z;
            BossRef->SetActorLocation(NewLocation, false);
        }
        break;
    }
    case 5:
    {
        FVector CurrentLocation = BossRef->GetActorLocation();
        FVector NewLocation = CurrentLocation - FVector(0, 0, BossRef->Attack2_DescendSpeed * DeltaTime);

        if (NewLocation.Z <= BossRef->Attack2_GroundZ)
        {
            NewLocation.Z = BossRef->Attack2_GroundZ;
            BossRef->SetActorLocation(NewLocation, false);

            if (BossRef->GetCharacterMovement())
            {
                BossRef->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
            }
            UE_LOG(LogTemp, Log, TEXT("Boss finished descending"));
            BossRef->SetState(EBossState::Chase);
        }
        else
        {
            BossRef->SetActorLocation(NewLocation, false);
        }
        break;
    }
    default:
        break;
    }
}

void UBoss_Attack2::DelayedFire_Attack2()
{
    if (!BossRef) return;

    UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
    if (AnimInst && AnimInst->Attack2Montage)
    {
        float Duration = BossRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimInst->Attack2Montage);
        UE_LOG(LogTemp, Log, TEXT("Attack2 montage playing for duration: %f"), Duration);
        BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Phase, this, &UBoss_Attack2::DelayedTransition, Duration, false);
    }
    else
    {
        BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Phase, this, &UBoss_Attack2::DelayedTransition, 2.0f, false);
    }
}

void UBoss_Attack2::DelayedTransition()
{
    if (!BossRef) return;
    BossRef->SetState(EBossState::Chase);
}

void UBoss_Attack2::ExitState()
{
    if (BossRef)
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Phase);
    }
    BossRef = nullptr;
    CurrentPhase = 6; // Completed
    UE_LOG(LogTemp, Log, TEXT("Boss is exiting ATTACK2 state"));
}
