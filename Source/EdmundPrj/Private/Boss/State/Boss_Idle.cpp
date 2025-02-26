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

    Boss->GetWorld()->GetTimerManager().SetTimer(IdleTimerHandle, FTimerDelegate::CreateLambda([Boss]()
        {
            if (Boss)
            {
                Boss->SetState(EBossState::Attack4);
            }
        }), 3.0f, false);
}

void UBoss_Idle::ExitState()
{
    UE_LOG(LogTemp, Log, TEXT("Idle End"));

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(IdleTimerHandle);
    }
}
