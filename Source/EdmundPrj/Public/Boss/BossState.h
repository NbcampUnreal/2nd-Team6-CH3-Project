#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BossState.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBossState : public UBTTaskNode
{
    GENERATED_BODY()

public:
    virtual void EnterState(ABoss* Boss) {}

    virtual void ExitState() {}

    virtual void TickState(float DeltaTime) {}
};
