#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossAttack1.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBTTask_BossAttack1 : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_BossAttack1();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnAnimationCompleted();

protected:
	ABoss* BossRef;

	UBehaviorTreeComponent* CachedOwnerComp;

};
