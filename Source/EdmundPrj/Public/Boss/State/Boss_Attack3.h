#pragma once

#include "CoreMinimal.h"
#include "Boss/BossState.h"
#include "Boss_Attack3.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBoss_Attack3 : public UBossState
{
    GENERATED_BODY()

public:
    UBoss_Attack3();

    virtual void EnterState(ABoss* Boss) override;
    virtual void ExitState() override;
    virtual void TickState(float DeltaTime) override;

protected:
    int32 ComboPhase;
    int32 RangedComboBulletCount;

    FTimerHandle TimerHandle_ComboPhase;
    FTimerHandle TimerHandle_RangedCombo;
    FTimerHandle TransitionTimerHandle;
    FVector TargetHorizontalLocation;

    UFUNCTION()
    void ExecuteMeleeAttack();

    UFUNCTION()
    void ExecuteRangedComboAttack();

    UFUNCTION()
    void EndRangedComboAttack();

    void PlayAttack3Montage();

    void DelayedTransition();

private:
    ABoss* BossRef = nullptr;
};
