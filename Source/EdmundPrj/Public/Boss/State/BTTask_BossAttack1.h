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

	// Behavior Tree 태스크 실행 시 호출
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// 타이머 콜백: 2초 대기 후 탄환 발사, 몽타주 재생, 회전 처리
	void DelayedFire();

	// 타이머 콜백: 일정 시간 대기 후 태스크 종료(예: 상태 전환)
	void DelayedTransition();

	// 탄환 발사 함수
	void FireBullet();

	// 타이머 핸들들
	FTimerHandle TimerHandle_Fire;
	FTimerHandle TimerHandle_Transition;

	// AI 컨트롤러가 소유한 보스 액터
	ABoss* BossRef;

	// 태스크 종료를 위해 보관하는 Behavior Tree 컴포넌트
	UBehaviorTreeComponent* CachedOwnerComp;

};
