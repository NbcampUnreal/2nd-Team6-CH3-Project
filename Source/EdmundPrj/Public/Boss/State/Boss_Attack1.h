#pragma once
#include "CoreMinimal.h"
#include "Boss/BossState.h"
#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Boss_Attack1.generated.h"

UCLASS()
class EDMUNDPRJ_API UBoss_Attack1 : public UBossState {
    GENERATED_BODY()

public:
    UBoss_Attack1();
    virtual void EnterState(ABoss* Boss) override;
    virtual void ExitState() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ABoss_Attack1_Bullet> BulletClass;

private:

    UPROPERTY()
    ABoss* BossRef;

    

    USceneComponent* MuzzleLocation;

    void FireBullet();
};