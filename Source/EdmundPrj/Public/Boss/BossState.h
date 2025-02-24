#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BossState.generated.h"

class ABoss;

// 전역 보스 상태 열거형
UENUM(BlueprintType)
enum class EBossState : uint8 {
    Idle        UMETA(DisplayName = "Idle"),
    Chase       UMETA(DisplayName = "Chase"),
    Attack1     UMETA(DisplayName = "Attack1"),
    Attack2     UMETA(DisplayName = "Attack2"),
    Attack3     UMETA(DisplayName = "Attack3"),
    Attack4     UMETA(DisplayName = "Attack4"),
    Skill1      UMETA(DisplayName = "Skill1"),
    Skill2      UMETA(DisplayName = "Skill2"),
    Skill3      UMETA(DisplayName = "Skill3"),
    Dead        UMETA(DisplayName = "Dead")
};

// 보스 상태 기본 클래스
UCLASS()
class EDMUNDPRJ_API UBossState : public UObject
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
