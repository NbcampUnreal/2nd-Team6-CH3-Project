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
    // ComboPhase: 1,2 = 근접 공격, 3 = 원거리 공격 (연발)
    int32 ComboPhase;
    // 원거리 콤보에서 발사한 총알 수
    int32 RangedComboBulletCount;

    // 타이머 핸들
    FTimerHandle TimerHandle_ComboPhase;
    FTimerHandle TimerHandle_RangedCombo;
    FTimerHandle TransitionTimerHandle;

    // 수평 이동 목표 위치 (플레이어 X, Y, 현재 Z 유지)
    FVector TargetHorizontalLocation;

    // 근접 공격 실행 (1타, 2타) – 이미 구현되어 있다고 가정
    UFUNCTION()
    void ExecuteMeleeAttack();

    // 원거리 콤보 공격 실행 (3타, 연속 5발)
    UFUNCTION()
    void ExecuteRangedComboAttack();

    // 원거리 콤보 공격이 끝난 후 상태 전환 (또는 다음 단계로)
    UFUNCTION()
    void EndRangedComboAttack();

    void PlayAttack3Montage();

    void DelayedTransition();

private:
    ABoss* BossRef = nullptr;
};
