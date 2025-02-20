#pragma once

#include "CoreMinimal.h"
#include "BossState.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Chase       UMETA(DisplayName = "Chase"),
    Attack1      UMETA(DisplayName = "Attack1"),
    Dead        UMETA(DisplayName = "Dead")
};

UCLASS()
class EDMUNDPRJ_API UBossState : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(class ABoss* Boss);
    void UpdateState(float DeltaTime);

private:
    class ABoss* BossCharacter;
    EBossState CurrentState;

    void SetState(EBossState NewState);
};
