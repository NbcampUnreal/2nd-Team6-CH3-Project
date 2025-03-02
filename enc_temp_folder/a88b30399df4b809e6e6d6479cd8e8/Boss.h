#pragma once

#include "CoreMinimal.h"
#include "Monster/BaseMonster.h"
#include "Boss/BossState.h"
#include "Boss/Boss_AnimInstance.h"
#include "Components/ArrowComponent.h"
#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Boss/Attack/Boss_Attack4_Bullet.h"
#include "Boss/Attack/Boss_Skill3_Wall.h"
#include "Particles/ParticleSystemComponent.h"
#include "Boss/State/BTTask_BossAttack2.h"
#include "Boss.generated.h"

class ABossAIController;
class AMissionHandle;


UCLASS()
class EDMUNDPRJ_API ABoss : public ABaseMonster {
    GENERATED_BODY()

public:
    ABoss();

#pragma region Capsule Components

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack3")
    USphereComponent* Attack2_MeleeCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* BossHeadCapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* NeckCapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Front_Right_FootCapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Front_Left_FootCapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Back_Right_FootCapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Back_Left_FootCapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Front_Right_LegCapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Front_Left_LegCapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Back_Right_LegCapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Back_Left_LegCapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Body1_CapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Body2_CapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Body3_CapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Tail1CapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Tail2CapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* Tail3CapsuleComponent;

#pragma endregion

    UPROPERTY(VisibleAnywhere, Category = "Collision")
    class USphereComponent* Attack2Collision;


    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    void SetState(EBossState NewState);
    void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);
    void InitiallizeBullerPool();
    float GetMonsterMoveSpeed() const { return MonsterMoveSpeed; }
    float SetMonsterMoveSpeed(float NewSpeed);
    void UpdateAttackCooldown(int32 AttackID);
    void MonsterAttackCheck() override;
    virtual void MonsterDead() override;
    float GetMonsterHP() const { return MonsterHP; }
    float GetMonsterMaxHP() const { return MonsterMaxHP; }
    int32 GetAttack1Count() const { return Attack1Count; }
    bool GetbSkill1Used() const { return bSkill1Used; }
    bool GetbSkill2Used() const { return bSkill2Used; }
    bool GetbSkill3Used() const { return bSkill3Used; }
    void SetbSkill1Used(bool bUsed) { bSkill1Used = bUsed; }
    void SetbSkill2Used(bool bUsed) { bSkill2Used = bUsed; }
    void SetbSkill3Used(bool bUsed) { bSkill3Used = bUsed; }
    float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
    int32 SetAttack1Count(int32 NewCount);
    void SetbIsInvulnerable(bool NewIsInvulnerable) { bIsInvulnerable = NewIsInvulnerable; }
    int32 GetComboPhase() { return ComboPhase; }
    void SetComboPhase(int32 NewComboPhase) { ComboPhase = NewComboPhase; }
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    bool GetbChaseComplete() const { return bChaseComplete; }
    void SetbChaseComplete(bool NewbChaseComplete) { bChaseComplete = NewbChaseComplete; }
    float GetBossSkill1Damage() const { return BossSkill1Damage; }
    void ActivateAttack2Collision();   
    void DeactivateAttack2Collision();
    void DisableMovement();
    void DisableRotation();
    void EnableMovement();
    void EnableRotation();
    float GetTurnSpeed() { return TurnSpeed; }


    UFUNCTION(BlueprintCallable)
    void SetCurrentAttackTask(UBTTask_BossAttack3* Task) { CurrentAttackTask = Task; }


    UFUNCTION()
    void OnAttack2CollisionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable)
    void FireBullet();

    UFUNCTION(BlueprintCallable)
    void HandleAttack2State(int32 State);

    UFUNCTION(BlueprintCallable)
    void OnAttack2Finished();

    UPROPERTY()
    class UBTTask_BossAttack2* BTTask_BossAttack2Instance;



private:
    UPROPERTY()
    UBossState* BossState;

    UPROPERTY()
    UBoss_AnimInstance* AnimInstance;

    int32 PoolSize = 15;
    int32 ComboPhase = 0;
    bool bSkill1Used = false;
    bool bSkill2Used = false;
    bool bSkill3Used = false;
    bool bIsInvulnerable = false;
    bool bChaseComplete = false;

public:
    // ***********************Stat*************************
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    float TurnSpeed = 5.0f; // 회전 속도

    float Chase_AcceptanceRadius = 60.0f; // Chase 반경

    // ***********************Attack 1*************************
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack1")
    float BossSkill1Damage = 1.0f;

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
    float Attack2_AscendSpeed = 30000.0f;   // 상승 속도

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
    float Attack2_DescendSpeed = 600.0f;   // 하강 속도

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
    float Attack2_TargetHeight = 10000.0f;  // 목표 높이 (Z)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
    float Attack2_HorizontalMoveSpeed = 500.0f; // 수평 이동 속도 (X, Y)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
    float Attack2_GroundZ = 0.0f;  // 하강 완료 기준 지면 높이

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* LandImpactParticle; // 착지 파티클

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    float KnockbackStrength = 25000.0f;  // 밀치는 힘

    // ***********************Attack 3*************************
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack3")
    float Attack3_CooldownEnd = 0.0f;

    UPROPERTY(BlueprintReadWrite)
    class UBTTask_BossAttack3* CurrentAttackTask;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
    float MeleeAttackDashDistance_Attack1 = 10000.0f; // 1타 돌진 거리

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
    float MeleeAttackDashDistance_Attack2 = 10000.0f; // 2타 돌진 거리

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
    float MeleeAttackDashSpeed_Attack1 = 1200.0f; // 1타 돌진 속도

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
    float MeleeAttackDashSpeed_Attack2 = 1300.0f; // 2타 돌진 속도

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UMaterialInterface* Skill2NewMaterial;



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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill3")
    float Skill3DetectionRadius = 1000.0f; // 감지 반경 (원형)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill3")
    UParticleSystemComponent* Skill3Particle; // 폭발 이펙트

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill3")
    float Skill3StartDelay = 3.0f; // 벽 스폰 후 딜레이 시간

    UPROPERTY(EditDefaultsOnly, Category = "Skill3")
    float Skill3AttackRadius = 1000.0f;  // 광역 공격 반경

    UPROPERTY(EditDefaultsOnly, Category = "Skill3")
    float Skill3AttackDamage = 50.0f;  // 광역 공격 데미지

    // ***********************Skill 4*************************



    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Cooldowns")
    float Attack1_CooldownDuration = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Cooldowns")
    float Attack2_CooldownDuration = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Cooldowns")
    float Attack3_CooldownDuration = 15.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Cooldowns")
    float Attack4_CooldownDuration = 20.0f;

    ////
    void InitBoss(AMissionHandle* NewMissionHandle);
    void CheckWeaken();
    void ApplyWeaken();

    TObjectPtr<ABossAIController> BossController = nullptr;
    TObjectPtr<AMissionHandle> MissionHandle = nullptr;
    
};

