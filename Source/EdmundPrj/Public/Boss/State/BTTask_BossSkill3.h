#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossSkill3.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBTTask_BossSkill3 : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_BossSkill3();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    void PlaySkillAnimation();
    void SpawnWall();
    void OnSpawnComplete();
    FRotator CalculateRandomRotation();

private:
    ABoss* BossRef;
    UBehaviorTreeComponent* CachedOwnerComp;
    FTimerHandle SpawnTimerHandle;
    int32 SpawnedWallCount;
};
