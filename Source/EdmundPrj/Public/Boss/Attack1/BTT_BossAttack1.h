#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossAttack1.generated.h"

UCLASS()
class EDMUNDPRJ_API UBTT_BossAttack1 : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_BossAttack1();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
