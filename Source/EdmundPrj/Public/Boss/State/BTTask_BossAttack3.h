#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossAttack3.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBTTask_BossAttack3 : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_BossAttack3();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(BlueprintReadOnly)
	class ABoss* BossRef;

	UFUNCTION(BlueprintCallable)
	void OnAttack1Notify();
	UFUNCTION(BlueprintCallable)
	void OnAttack2Notify();
	UFUNCTION(BlueprintCallable)
	void FinishComboAttack();
	void ExecuteMeleeAttack();
	// 몽타주 재생 함수
	void PlayAttack3Montage();

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	int32 GetComboPhase() { return ComboPhase; }
	
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;

	int32 ComboPhase;
};
