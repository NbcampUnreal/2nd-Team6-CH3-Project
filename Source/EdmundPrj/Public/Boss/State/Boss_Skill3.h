#pragma once

#include "CoreMinimal.h"
#include "Boss/BossState.h"
#include "Boss_Skill3.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBoss_Skill3 : public UBossState
{
    GENERATED_BODY()

public:
    UBoss_Skill3();

    virtual void EnterState(ABoss* Boss) override;
    virtual void ExitState() override;
    virtual void TickState(float DeltaTime) override;

protected:
    // 애니메이션 재생 함수
    void PlaySkillAnimation();

    // 벽 소환 함수 (한 번 소환)
    void SpawnWall();

    // 소환 반복 타이머 종료 후 상태 전환 함수
    void OnSpawnComplete();

    FRotator CalculateRandomRotation();

private:
    ABoss* BossRef = nullptr;
    FTimerHandle SpawnTimerHandle;
    int32 SpawnedWallCount = 0;
};
