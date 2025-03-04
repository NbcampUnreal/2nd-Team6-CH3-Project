#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossSkill2.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBTTask_BossSkill2 : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_BossSkill2();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    UPROPERTY()
    UBehaviorTreeComponent* CachedOwnerComp = nullptr;
    void PlaySkill2Animation();
    UPROPERTY()
    ABoss* BossRef = nullptr;

private:
    UMaterialInterface* OriginalMaterial = nullptr;
public:

};
