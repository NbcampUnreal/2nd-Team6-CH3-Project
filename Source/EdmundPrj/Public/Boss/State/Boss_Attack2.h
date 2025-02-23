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

    // 상태 진입 및 종료
    virtual void EnterState(ABoss* Boss) override;
    virtual void ExitState() override;

    // 매 프레임 호출되는 상태 업데이트 함수
    virtual void TickState(float DeltaTime) override;

protected:
    // 내부 단계는 int32로 관리합니다.
    // 0: InitialWait  
    // 1: Ascending  
    // 2: AtMaxWait  
    // 3: HorizontalReposition  
    // 4: PreDescendWait  
    // 5: Descending  
    // 6: Completed
    int32 CurrentPhase;

    // 보스 참조
    ABoss* BossRef = nullptr;

    // 대기 단계 타이머 핸들
    FTimerHandle TimerHandle_Phase;

    // 수평 이동 목표 위치 (플레이어 X,Y, 현재 Z 유지)
    FVector TargetHorizontalLocation;

    // 내부 단계 전환 함수
    void StartAscend();
    void OnAscendComplete();
    void StartDescend();
};
