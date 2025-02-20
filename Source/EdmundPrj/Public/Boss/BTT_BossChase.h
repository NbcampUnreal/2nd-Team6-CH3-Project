#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossChase.generated.h"

UCLASS()
class EDMUNDPRJ_API UBTT_BossChase : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_BossChase();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};