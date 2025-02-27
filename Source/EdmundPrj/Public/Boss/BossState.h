#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BossState.generated.h"

class ABoss;

// 전역 보스 상태 열거형


// 보스 상태 기본 클래스
UCLASS()
class EDMUNDPRJ_API UBossState : public UBTTaskNode
{
    GENERATED_BODY()

public:
    // 상태에 진입할 때 호출
    virtual void EnterState(ABoss* Boss) {}

    // 상태를 벗어날 때 호출
    virtual void ExitState() {}

    // 매 프레임 상태 업데이트 (보스의 Tick에서 호출)
    virtual void TickState(float DeltaTime) {}
};
