#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossSkill3.generated.h"

class ABoss;
class ABoss_Skill3_Wall; // 🔹 추가: 전방 선언 (Wall 클래스 인식 문제 해결)

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
    void PerformAOEAttack();
    FRotator CalculateRandomRotation();
    bool CheckWallBlocking(UWorld* World, FVector BossLocation, FVector HitLocation, AActor* Wall, FCollisionQueryParams QueryParams);

private:
    ABoss* BossRef;
    UBehaviorTreeComponent* CachedOwnerComp;
    FTimerHandle SpawnTimerHandle;
    int32 SpawnedWallCount;
};
