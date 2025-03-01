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

	// 행동 트리 태스크 실행 시 호출됩니다.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 애님 노티파이에 의해 진행되는 콜백 함수들
	void OnAttack1Notify();  // 1타 노티파이(BossAttack3_1) 호출 시
	void OnAttack2Notify();  // 2타 노티파이(BossAttack3_2) 호출 시
	void FinishComboAttack(); // 3타 노티파이(BossAttack3_3) 호출 시, 콤보 종료
	void ExecuteMeleeAttack();
	// 몽타주 재생 함수
	void PlayAttack3Montage();

	// 필요하다면 TickTask도 구현 가능 (현재는 애니메이션 노티파이에 의존)
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	int32 GetComboPhase() { return ComboPhase; }
	

protected:
	// 보스 캐릭터 참조
	UPROPERTY()
	ABoss* BossRef;

	// 행동 트리 컴포넌트 캐시
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;

	// 콤보 진행 단계 (0: 초기, 1: 1타 종료 후, 2: 2타 종료 후)
	int32 ComboPhase;
};
