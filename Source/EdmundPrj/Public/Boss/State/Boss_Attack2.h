#pragma once

#include "CoreMinimal.h"
#include "Boss/BossState.h"
#include "Boss_Attack2.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBoss_Attack2 : public UBossState
{
    GENERATED_BODY()

public:
    UBoss_Attack2();

    virtual void EnterState(ABoss* Boss) override;
    virtual void ExitState() override;
    virtual void TickState(float DeltaTime) override;

    UFUNCTION()
    void DelayedFire_Attack2();

    UFUNCTION()
    void DelayedTransition();

protected:
    // 0: InitialWait  
    // 1: Ascending  
    // 2: AtMaxWait  
    // 3: HorizontalReposition  
    // 4: PreDescendWait  
    // 5: Descending  
    // 6: Completed
    int32 CurrentPhase;


    ABoss* BossRef = nullptr;

    FTimerHandle TimerHandle_Phase;
    FVector TargetHorizontalLocation;

    void StartAscend();
    void OnAscendComplete();
    void StartDescend();
};
