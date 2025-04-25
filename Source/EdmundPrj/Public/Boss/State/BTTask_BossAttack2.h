#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossAttack2.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBTTask_BossAttack2 : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_BossAttack2();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void OnAttack2Completed();

protected:
	int32 CurrentPhase;
	ABoss* BossRef;
	UBehaviorTreeComponent* CachedOwnerComp;
	FTimerHandle TimerHandle_Phase;
	FVector TargetHorizontalLocation;
	FTimerHandle TimerHandle_DisableCollision;

	void StartAscend();
	void OnAscendComplete();
	void StartDescend();
	void DelayedFire_Attack2();
	void DelayedTransition();

	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};
