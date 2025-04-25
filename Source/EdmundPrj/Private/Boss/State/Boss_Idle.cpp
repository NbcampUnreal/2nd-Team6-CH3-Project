#include "Boss/State/Boss_Idle.h"
#include "Boss/Boss.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UBoss_Idle::EnterState(ABoss* Boss)
{
    Super::EnterState(Boss);

    if (!Boss || !Boss->GetWorld())
    {
        return;
    }

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

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(IdleTimerHandle);
    }
}
