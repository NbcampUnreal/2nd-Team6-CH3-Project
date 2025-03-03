#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossInitialize.generated.h"

UCLASS()
class EDMUNDPRJ_API UBTTask_BossInitialize : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_BossInitialize();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
    class ABoss* BossCharacter;
    UPROPERTY()
    UBehaviorTreeComponent* OwnerCompRef;
};
