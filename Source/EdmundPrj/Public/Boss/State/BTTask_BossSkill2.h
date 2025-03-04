#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossSkill2.generated.h"

class ABoss;

/**
 * UBTTask_BossSkill2
 * 기존 몬스터 스폰 기능 대신 보스의 스켈레탈 메시를 변경하는 동작을 수행하는 태스크.
 */
UCLASS()
class EDMUNDPRJ_API UBTTask_BossSkill2 : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_BossSkill2();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    void PlaySkill2Animation();

    UPROPERTY()
    UBehaviorTreeComponent* CachedOwnerComp = nullptr;

    UPROPERTY()
    ABoss* BossRef = nullptr;

private:
    UMaterialInterface* OriginalMaterial = nullptr;
    FTimerHandle TimerHandle;
public:

};
