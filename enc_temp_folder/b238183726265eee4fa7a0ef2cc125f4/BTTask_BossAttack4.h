#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossAttack4.generated.h"

class ABoss;
class UBehaviorTreeComponent;
class UAnimMontage;

UCLASS()
class EDMUNDPRJ_API UBTTask_BossAttack4 : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_BossAttack4();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    // 단계 정의:
    // 0: 대기 (WaitBeforeRise)
    // 1: 상승 (Rising)
    // 2: 대기 (WaitAtTarget) - 상승 완료 후 잠시 대기
    // 3: 탄환 발사 (BulletFire)
    // 4: 하강 (Descending)
    // 5: 대기 (WaitAfterLanding) 후 태스크 종료
    int32 CurrentPhase;
    FTimerHandle TimerHandle_Phase;
    FTimerHandle TimerHandle_BulletFire;
    FTimerHandle TimerHandle_Transition;

    ABoss* BossRef;

    UBehaviorTreeComponent* CachedOwnerComp;


    void StartRise();
    void UpdateRise(float DeltaTime);
    void OnRiseComplete();

    void StartBulletFire();
    void ExecuteBulletFire();
    void EndBulletFire();

    void UpdateDescend(float DeltaTime);
    void OnDescendComplete();

    void DelayedTransition();
};
