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
};
