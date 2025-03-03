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
	// Phase 정의:
	// 0: InitialWait (대기)
	// 1: Ascending (상승)
	// 2: AtMaxWait (최대 높이 도달 후 대기)
	// 3: HorizontalReposition (플레이어 위치 이동)
	// 4: PreDescendWait (대기)
	// 5: Descending (하강)
	// 6: Completed (종료)

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
