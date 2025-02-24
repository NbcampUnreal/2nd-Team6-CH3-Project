#pragma once

#include "CoreMinimal.h"
#include "Boss/BossState.h"
#include "Boss_Attack4.generated.h"

class ABoss;
class UAnimMontage;

UCLASS()
class EDMUNDPRJ_API UBoss_Attack4 : public UBossState
{
    GENERATED_BODY()

public:
    UBoss_Attack4();

    virtual void EnterState(ABoss* Boss) override;
    virtual void ExitState() override;
    virtual void TickState(float DeltaTime) override;

protected:
    // 단계 정의:
    // 0: WaitBeforeRise (1초 대기)
    // 1: Rising (상승)
    // 2: WaitAtTarget (타겟 높이 도달 후 2초 대기)
    // 3: BulletFire (탄환 발사, Attack4 몽타주 재생 및 유도 탄환 발사)
    // 4: Descending (하강)
    // 5: WaitAfterLanding (착지 후 1초 대기)
    int32 CurrentPhase;

    // 보스 참조
    ABoss* BossRef = nullptr;

    // 타이머 핸들
    FTimerHandle TimerHandle_Phase;
    FTimerHandle TimerHandle_BulletFire;
    FTimerHandle TransitionTimerHandle;

    // 공격4 관련 조절 변수 (보스 블루프린트에서 조정 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Movement")
    float WaitBeforeRise = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Movement")
    float Attack4_RiseSpeed = 400.0f;  // 상승 속도

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Movement")
    float Attack4_TargetHeight = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Movement")
    float WaitAtTarget = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Bullet")
    float FireDuration = 1.5f;  // 탄환 발사 지속 시간

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Bullet")
    float FireInterval = 0.2f;  // 탄환 발사 간격

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Bullet")
    int32 FireBulletCount = 5;  // 발사할 탄환 수

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Bullet")
    float Attack4_HomingDuration = 2.0f;  // 탄환 유도 지속 시간

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Movement")
    float Attack4_DescendSpeed = 600.0f;  // 하강 속도

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Movement")
    float Attack4_GroundZ = 0.0f;         // 착지 기준 높이

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Movement")
    float WaitAfterLanding = 1.0f;

    // Attack4 몽타주 (애님 인스턴스에서 할당)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Animation")
    UAnimMontage* Attack4Montage;

    // 내부 함수
    void StartRise();
    void UpdateRise(float DeltaTime);
    void OnRiseComplete();

    void StartBulletFire();
    void ExecuteBulletFire(); // 탄환 발사 로직 (풀링 방식, Attack1Bullet 사용)
    void EndBulletFire();

    void StartDescend();
    void UpdateDescend(float DeltaTime);
    void OnDescendComplete();

    void PlayAttack4Montage(); // 몽타주 재생
    void DelayedTransition();   // 최종 대기 후 상태 전환
};
