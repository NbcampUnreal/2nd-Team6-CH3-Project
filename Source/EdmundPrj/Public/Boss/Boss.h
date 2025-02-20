#pragma once

#include "CoreMinimal.h"
#include "Monster/BaseMonster.h"
#include "Boss/BossState.h"
#include "Boss.generated.h"

UCLASS()
class EDMUNDPRJ_API ABoss : public ABaseMonster
{
    GENERATED_BODY()

public:
    ABoss();
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UBossState* BossState;
};