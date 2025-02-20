#include "Boss/State/Boss_Idle.h"
#include "Boss/Boss.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UBoss_Idle::EnterState(ABoss* Boss)
{
    Super::EnterState(Boss);

    if (!Boss || !Boss->GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("Boss or World is NULL in Idle State"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Idle Start"));

    // 5초 후 Chase 상태로 변경하는 타이머 설정
    Boss->GetWorld()->GetTimerManager().SetTimer(IdleTimerHandle, FTimerDelegate::CreateLambda([Boss]()
        {
            if (Boss)
            {
                UE_LOG(LogTemp, Log, TEXT("Idle Timer Triggered, Changing to Chase"));
                Boss->SetState(EBossState::Chase);
            }
        }), 5.0f, false);

    UE_LOG(LogTemp, Log, TEXT("Idle Timer Set Successfully"));
}

void UBoss_Idle::ExitState()
{
    UE_LOG(LogTemp, Log, TEXT("Idle End"));

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(IdleTimerHandle);
    }
}
