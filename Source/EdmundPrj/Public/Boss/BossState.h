#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BossState.generated.h"

class ABoss;

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

UCLASS()
class EDMUNDPRJ_API UBossState : public UObject {
    GENERATED_BODY()

public:
    virtual void EnterState(ABoss* Boss) {}
    virtual void ExitState() {}
};
