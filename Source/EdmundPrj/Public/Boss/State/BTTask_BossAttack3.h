#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossAttack3.generated.h"

class ABoss;
class UBehaviorTreeComponent;

UCLASS()
class EDMUNDPRJ_API UBTTask_BossAttack3 : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_BossAttack3();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	int32 ComboPhase;
	int32 RangedComboBulletCount;
	float CurrentPhaseTimer;

	static constexpr float InitialDelay = 1.0f;          // Phase 0 대기
	static constexpr float MeleeAttackInterval = 2.5f;     // Phase 1,2: 근접 공격 후 대기
	static constexpr float RangedAttackInterval = 0.2f;    // Phase 3: 0.2초마다 탄환 발사

	void ExecuteMeleeAttack();
	void ExecuteRangedComboAttack();
	void FinishComboAttack();
	void PlayAttack3Montage();

	UBehaviorTreeComponent* CachedOwnerComp;
	ABoss* BossRef;
};
