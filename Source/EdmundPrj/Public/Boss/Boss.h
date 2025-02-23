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
    float GetMonsterMoveSpeed() const { return MonsterMoveSpeed; }
    float SetMonsterMoveSpeed(float NewSpeed);
    int32 GetAttack1Count() const { return Attack1Count; }
    int32 SetAttack1Count(int32 NewCount);


private:
    UPROPERTY()
    UBossState* BossState;

    UPROPERTY()
    UBoss_AnimInstance* AnimInstance;

public:

    // ***********************Attack 1*************************
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Attack1Count = 0;

    // 탄환 발사 위치 (Muzzle)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack1")
    UArrowComponent* MuzzleLocation;

    // Attack1 탄환 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack1")
    TSubclassOf<ABoss_Attack1_Bullet> Attack1BulletClass;


    // ***********************Attack 2*************************
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
    float Attack2_AscendSpeed = 300.0f;   // 상승 속도

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
    float Attack2_DescendSpeed = 600.0f;   // 하강 속도

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
    float Attack2_TargetHeight = 1000.0f;  // 목표 높이 (Z)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
    float Attack2_HorizontalMoveSpeed = 500.0f; // 수평 이동 속도 (X, Y)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
    float Attack2_GroundZ = 0.0f;  // 하강 완료 기준 지면 높이

    // ***********************Attack 3*************************
};
