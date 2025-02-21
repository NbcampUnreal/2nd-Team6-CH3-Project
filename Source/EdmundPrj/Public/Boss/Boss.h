#pragma once
#include "CoreMinimal.h"
#include "Monster/BaseMonster.h"
#include "Boss/BossState.h"
#include "Boss/Boss_AnimInstance.h"
#include "Boss.generated.h"

UCLASS()
class EDMUNDPRJ_API ABoss : public ABaseMonster {
    GENERATED_BODY()

public:
    ABoss();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    void SetState(EBossState NewState);

private:
    UPROPERTY()
    UBossState* BossState;

    UPROPERTY()
    UBoss_AnimInstance* AnimInstance;

};