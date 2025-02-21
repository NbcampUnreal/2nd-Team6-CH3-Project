#pragma once
#include "CoreMinimal.h"
#include "Boss/BossState.h"
#include "Boss_Idle.generated.h"

UCLASS()
class EDMUNDPRJ_API UBoss_Idle : public UBossState {
    GENERATED_BODY()

private:
    FTimerHandle IdleTimerHandle;

public:
    virtual void EnterState(ABoss* Boss) override;
    virtual void ExitState() override;
};
