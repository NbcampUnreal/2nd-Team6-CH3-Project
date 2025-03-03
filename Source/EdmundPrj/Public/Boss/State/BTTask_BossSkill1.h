#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossSkill1.generated.h"

class ABoss;
class UBehaviorTreeComponent;

UCLASS()
class EDMUNDPRJ_API UBTTask_BossSkill1 : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_BossSkill1();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    ABoss* BossRef = nullptr;
    UBehaviorTreeComponent* CachedOwnerComp;
    int32 CurrentOverlapCount;

    FTimerHandle StartWaitTimerHandle; // 초기 대기
    FTimerHandle OverlapTimerHandle;   // 오버랩
    FTimerHandle GapTimerHandle;       // 대기

private:
    void PlaySkill1Animation();
    void OnStartWaitComplete();
    void StartOverlap();
    void PerformOverlapCheck(bool bFloorPattern);
    void OnOverlapEnd();
    void OnGapEnd();
    void EndTask();
};
