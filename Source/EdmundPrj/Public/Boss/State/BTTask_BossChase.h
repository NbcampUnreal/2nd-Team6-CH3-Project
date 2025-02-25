#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossChase.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBTTask_BossChase : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_BossChase();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	ABoss* BossRef;
	UBehaviorTreeComponent* CachedOwnerComp;
	float AccumulatedTime;

};
