#pragma once
#include "CoreMinimal.h"
#include "Monster/BaseMonster.h"
#include "Boss/BossState.h"
#include "Boss/Boss_AnimInstance.h"
#include "Components/ArrowComponent.h"
#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Boss.generated.h"

UCLASS()
class EDMUNDPRJ_API ABoss : public ABaseMonster {
    GENERATED_BODY()

public:
    ABoss();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    void SetState(EBossState NewState);

    void InitiallizeBullerPool();

private:
    UPROPERTY()
    UBossState* BossState;

    UPROPERTY()
    UBoss_AnimInstance* AnimInstance;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UArrowComponent* MuzzleLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack1")
    TSubclassOf<ABoss_Attack1_Bullet> Attack1BulletClass;

};