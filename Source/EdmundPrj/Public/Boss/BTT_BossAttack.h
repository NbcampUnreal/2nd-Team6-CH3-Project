#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossAttack.generated.h"

UCLASS()
class EDMUNDPRJ_API UBTT_BossAttack : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_BossAttack();

protected:
    //virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
