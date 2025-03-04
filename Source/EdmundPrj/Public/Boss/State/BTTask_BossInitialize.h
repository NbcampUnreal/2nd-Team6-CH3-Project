#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossInitialize.generated.h"

class ABoss;
class UBehaviorTreeComponent;
class UAnimMontage;

UCLASS()
class EDMUNDPRJ_API UBTTask_BossInitialize : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_BossInitialize();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
    ABoss* BossCharacter;
    UBehaviorTreeComponent* OwnerCompRef;
    bool bDescentSpeedCalculated;
    float DescentSpeed;
    FVector TargetGroundLocation;
};
