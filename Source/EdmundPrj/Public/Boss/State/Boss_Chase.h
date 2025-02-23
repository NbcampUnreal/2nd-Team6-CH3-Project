#pragma once
#include "CoreMinimal.h"
#include "Boss/BossState.h"
#include "Boss_Chase.generated.h"

UCLASS()
class EDMUNDPRJ_API UBoss_Chase : public UBossState {
    GENERATED_BODY()

private:
    FTimerHandle UpdateTimerHandle;
    ABoss* BossCharacter;

public:
    virtual void EnterState(ABoss* Boss) override;
    virtual void ExitState() override;
    void UpdateChase();
};
