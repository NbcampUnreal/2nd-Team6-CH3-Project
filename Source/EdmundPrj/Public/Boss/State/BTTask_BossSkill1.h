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
    void OnSkill1MontageEnded(UAnimMontage* Montage, bool bInterrupted);
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    ABoss* BossRef = nullptr;
    UBehaviorTreeComponent* CachedOwnerComp = nullptr;

    int32 AttackIterationCount = 0;
    FTimerHandle InitialMontageTimerHandle;
    FTimerHandle AttackIterationTimerHandle;

private:
    void PlayInitialMontage();
    void StartAttackPattern();
    void PerformAttackIteration();
    void OnAttackHintFinished(bool bIsHeadAttack);
    void OnAttackIterationEnd();
    void PerformOverlapCheck(bool bFloorPattern);
    void EndTask();
};
