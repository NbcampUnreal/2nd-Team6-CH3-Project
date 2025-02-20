#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BTT_BossAttack2.generated.h"

UCLASS()
class EDMUNDPRJ_API UBTT_BossAttack2 : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_BossAttack2();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UPROPERTY(EditAnywhere, Category = "Movement")
    float RiseSpeed = 5.0f;  // 상승 속도

    UPROPERTY(EditAnywhere, Category = "Movement")
    float FallSpeed = 5.0f;  // 하강 속도

    UPROPERTY(EditAnywhere, Category = "Movement")
    float RiseHeight = 5000.0f; // 상승 높이

    float RiseAlpha = 0.0f; // 상승 진행도
    float FallAlpha = 0.0f; // 하강 진행도

    FVector StartLocation;
    FVector RiseTarget;

    FTimerHandle GroundWaitTimerHandle;
    FTimerHandle RiseTimerHandle;
    FTimerHandle AirWaitTimerHandle;
    FTimerHandle FallTimerHandle;

    void StartRising(AActor* BossActor, UBehaviorTreeComponent* OwnerComp);
    void UpdateRising(AActor* BossActor, UBehaviorTreeComponent* OwnerComp);
    void StartAirWait(AActor* BossActor, UBehaviorTreeComponent* OwnerComp);
    void StartFalling(AActor* BossActor, UBehaviorTreeComponent* OwnerComp);
    void UpdateFalling(AActor* BossActor, FVector FallStartLocation, FVector FallTarget, UBehaviorTreeComponent* OwnerComp);
    void FinishTask(UBehaviorTreeComponent* OwnerComp);
};
