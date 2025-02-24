#pragma once

#include "CoreMinimal.h"
#include "Boss/BossState.h"  // 기존 상태 기본 클래스
#include "Boss_Attack1.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBoss_Attack1 : public UBossState
{
	GENERATED_BODY()

public:
	UBoss_Attack1();

	virtual void EnterState(ABoss* Boss) override;
	virtual void ExitState() override;

	// 탄환 발사를 수행하는 함수
	void FireBullet();

	// 2초 후에 탄환을 발사하는 함수 (타이머 콜백)
	void DelayedFire();

	// 탄환 발사 후 2초 대기 후 다음 상태(Chase)로 전환하는 함수 (타이머 콜백)
	void DelayedTransition();

private:
	// 현재 보스 참조
	ABoss* BossRef = nullptr;

	// 타이머 핸들 (두 타이머 모두 멤버 변수로 관리)
	FTimerHandle RotationTimerHandle;
	FTimerHandle TransitionTimerHandle;

};
