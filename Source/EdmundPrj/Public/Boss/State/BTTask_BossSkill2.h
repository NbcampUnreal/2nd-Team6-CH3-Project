#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossSkill2.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBTTask_BossSkill2 : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_BossSkill2();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    // TickTask()가 별도로 필요 없으므로 생략할 수 있습니다.
    // virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    // 타이머 핸들: 2초 후 몬스터 소환 및 태스크 완료
    FTimerHandle SpawnTimerHandle;

    // Boss 참조 (AI 컨트롤러의 Pawn)
    ABoss* BossRef;

    // Behavior Tree 컴포넌트 캐시
    UBehaviorTreeComponent* CachedOwnerComp;

    // 내부 함수들
    void PlaySkill2Animation();
    void SpawnMonsters();

    // 타이머 콜백: 스킬 실행 완료 후 호출
    void OnSkill2Complete();
};
