#pragma once

#include "CoreMinimal.h"
#include "Monster/BaseMonster.h"
#include "Boss/BossState.h"
#include "Boss/Boss_AnimInstance.h"
#include "Components/ArrowComponent.h"
#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Boss/Attack/Boss_Attack4_Bullet.h"
#include "Boss/Attack/Boss_Skill3_Wall.h"
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
    void UpdateAttackCooldown(int32 AttackID);
    void MonsterAttackCheck() override;

    int32 GetAttack1Count() const { return Attack1Count; }
    int32 SetAttack1Count(int32 NewCount);


private:
    UPROPERTY()
    UBossState* BossState;

    UPROPERTY()
    UBoss_AnimInstance* AnimInstance;
    

private:
    int32 PoolSize = 15;

public:
    float Chase_AcceptanceRadius = 1000.0f; // Chase 반경

    // ***********************Attack 1*************************
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack1")
    float Attack1_CooldownEnd = 0.0f;

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
    float Attack2_CooldownEnd = 0.0f;

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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack3")
    float Attack3_CooldownEnd = 0.0f;

    // ***********************Attack 4*************************
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    float Attack4_CooldownEnd = 0.0f;

    // Attack4 탄환 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    TSubclassOf<ABoss_Attack4_Bullet> Attack4BulletClass;

    // 탄환 발사 횟수를 관리하는 변수 (현재 발사된 탄환 수)
    int32 Attack4_Attack4FiredCount = 0;

    // 상승 전 대기 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    float Attack4_WaitBeforeRise = 1.0f;

    // 상승 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    float Attack4_RiseSpeed = 400.0f;

    // 목표 높이
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    float Attack4_TargetHeight = 1500.0f;

    // 목표 높이에 도달한 후, 탄환 발사 전에 대기하는 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    float Attack4_WaitAtTarget = 2.0f;

    // 탄환 발사 지속 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    float Attack4_FireDuration = 1.5f;

    // 탄환 발사 간격
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    float Attack4_FireInterval = 0.2f;

    // 발사할 탄환의 총 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    int32 Attack4_FireBulletCount = 5;

    // 유도 지속 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    float Attack4_HomingDuration = 2.0f;

    // 하강 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    float Attack4_DescendSpeed = 600.0f;

    // 착지 후 대기 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4")
    float Attack4_WaitAfterLanding = 1.0f;


    // ***********************Skill 1*************************
    // ***********************Skill 2*************************

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill2")
    float Skill2SpawnRadius = 500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill2")
    int32 Skill2SpawnCount = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill2")
    TArray<TSubclassOf<AActor>> Skill2MonsterList;

    // ***********************Skill 3*************************
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill3")
    int32 Skill3WallCount = 3; // 소환할 벽의 개수 max = 10

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill3")
    float Skill3SpawnRadius = 2500.f; // 보스 주변 최대 소환 반경

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill3")
    float Skill3MinSpawnDistance = 1300.f; // 최소 소환 거리

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill3")
    float Skill3SpawnInterval = 0.5f; // 벽 소환 간격 (초)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill3")
    TSubclassOf<ABoss_Skill3_Wall> Skill3WallClass; // 소환할 벽 BP 클래스

    // ***********************Skill 4*************************



    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Cooldowns")
    float Attack1_CooldownDuration = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Cooldowns")
    float Attack2_CooldownDuration = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Cooldowns")
    float Attack3_CooldownDuration = 15.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Cooldowns")
    float Attack4_CooldownDuration = 20.0f;

};

