#pragma once

#include "CoreMinimal.h"
#include "Boss/BossState.h"
#include "Boss_Skill2.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBoss_Skill2 : public UBossState
{
    GENERATED_BODY()

public:
    UBoss_Skill2();

    virtual void EnterState(ABoss* Boss) override;
    virtual void ExitState() override;
    virtual void TickState(float DeltaTime) override;

protected:
    void PlaySkill2Animation();
    void SpawnMonsters();

private:
    ABoss* BossRef = nullptr;
};
