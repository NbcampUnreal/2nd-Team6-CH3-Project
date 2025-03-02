#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossSkill3.generated.h"

class ABoss;
class ABoss_Skill3_Wall; // 추가: 전방 선언

UCLASS()
class EDMUNDPRJ_API UBTTask_BossSkill3 : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_BossSkill3();
    bool bHasAttacked;
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    void PlaySkillAnimation();
    void SpawnWall();
    void OnSpawnComplete();
    FRotator CalculateRandomRotation();

    void StartDetection();
    void PerformDetection();
    void StopDetection();
    FTimerHandle DetectionTimer;
    FTimerHandle EndTimer;
    FTimerHandle DelayedStartTimer;

private:
    ABoss* BossRef;
    UBehaviorTreeComponent* CachedOwnerComp;
    FTimerHandle SpawnTimerHandle;
    int32 SpawnedWallCount;
};
