#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NPCTrace.generated.h"

class ANPC;

UCLASS()
class EDMUNDPRJ_API UBTTask_NPCTrace : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_NPCTrace();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	ANPC* NPCRef;
	UBehaviorTreeComponent* CachedOwnerComp;
};
