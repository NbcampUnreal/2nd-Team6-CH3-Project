#include "Boss/State/Boss_Attack2.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h" // 추가
#include "Engine/World.h"

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

    // 초기 2초 대기 후 상승 시작
    BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Phase, [this]()
        {
            StartAscend();
        }, 2.0f, false);
}

void UBoss_Attack2::StartAscend()
{
    UE_LOG(LogTemp, Log, TEXT("Boss starting to ascend"));

    // 이동 모드를 Flying으로 전환하여 Z축 이동 가능하게 함
    if (BossRef && BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
    }

    CurrentPhase = 1; // Ascending
    // 상승은 TickState()에서 매 프레임 처리합니다.
}

void UBoss_Attack2::OnAscendComplete()
{
    UE_LOG(LogTemp, Log, TEXT("Boss reached target height, waiting 1 second at max height"));
    CurrentPhase = 2; // AtMaxWait

    BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Phase, [this]()
        {
            // 수평 재배치 단계 시작 (플레이어의 X,Y, 현재 Z 유지)
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
    CurrentPhase = 5; // Descending
    // 하강은 TickState()에서 처리합니다.
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
            // Sweep 옵션을 false로 하여 충돌 검사 없이 위치를 강제 업데이트
            BossRef->SetActorLocation(NewLocation, false);
            CurrentPhase = 6; // 임시 Completed 상태로 전환
            OnAscendComplete();
        }
        else
        {
            BossRef->SetActorLocation(NewLocation, false);
        }
        break;
    }
    case 3: // HorizontalReposition
    {
        FVector CurrentLocation = BossRef->GetActorLocation();
        FVector Direction = (TargetHorizontalLocation - CurrentLocation);
        Direction.Z = 0; // Z는 유지
        float Distance = Direction.Size();

        if (Distance < 10.0f)
        {
            BossRef->SetActorLocation(FVector(TargetHorizontalLocation.X, TargetHorizontalLocation.Y, CurrentLocation.Z));
            CurrentPhase = 4; // PreDescendWait
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
    case 5: // Descending
    {
        FVector CurrentLocation = BossRef->GetActorLocation();
        FVector NewLocation = CurrentLocation - FVector(0, 0, BossRef->Attack2_DescendSpeed * DeltaTime);

        if (NewLocation.Z <= BossRef->Attack2_GroundZ)
        {
            NewLocation.Z = BossRef->Attack2_GroundZ;
            BossRef->SetActorLocation(NewLocation, false);

            // 하강 완료 시 다시 Walking 모드로 전환
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
        // 0 (InitialWait), 2 (AtMaxWait), 4 (PreDescendWait), 6 (Completed) 등은 타이머로 전환
        break;
    }
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
