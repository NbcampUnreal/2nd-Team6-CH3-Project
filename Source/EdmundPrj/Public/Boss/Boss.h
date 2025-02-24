#pragma once

#include "CoreMinimal.h"
#include "Monster/BaseMonster.h"
#include "Boss/BossState.h"
#include "Boss/Boss_AnimInstance.h"
#include "Components/ArrowComponent.h"
#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Boss/Attack/Boss_Attack4_Bullet.h"
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

private:
    int32 PoolSize = 5;

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


    // ***********************Attack 4*************************
    // Attack4 탄환 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    TSubclassOf<ABoss_Attack4_Bullet> Attack4BulletClass;

    // ***********************Skill 1*************************
    // ***********************Skill 2*************************

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill2")
    float Skill2SpawnRadius = 500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill2")
    int32 Skill2SpawnCount = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill2")
    TArray<TSubclassOf<AActor>> Skill2MonsterList;

    // ***********************Skill 3*************************
    // ***********************Skill 4*************************
};
