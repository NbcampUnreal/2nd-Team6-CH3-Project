#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Boss/State/BTTask_BossAttack3.h"  
#include "BossAIController.generated.h"

class ABoss;
class AMissionHandle;

UCLASS()
class EDMUNDPRJ_API ABossAIController : public AAIController
{
    GENERATED_BODY()

public:
    ABossAIController();

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;
    //void OnAttackTaskComplete(int32 AttackID);
    void InitBlackboard(AMissionHandle* NewMissionHandle);
    void NotifyClearHalfPattern();

    UPROPERTY(BlueprintReadWrite, Category = "Attack")
    UBTTask_BossAttack3* CurrentAttackTask;
    
private:
    bool CheckHpPattern();
    void CheckBossHeight();
    void CheckLockedSkill(float CurrentTime);
    void EnableHalfPattern();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss")
    ABoss* BossCharacter;
      
private:
    TObjectPtr<UBlackboardComponent> BBComp = nullptr;
    TObjectPtr<AMissionHandle> MissionHandle = nullptr;
    bool bIsStart = false;
    bool bIsEnableHalf = false;

    bool bAttack1Ready = false;
    bool bAttack2Ready = false;
    bool bAttack3Ready = false;
    bool bAttack4Ready = false;

    float Attack1_CooldownEnd = 0.0f;
    float Attack2_CooldownEnd = 0.0f;
    float Attack3_CooldownEnd = 0.0f;
    float Attack4_CooldownEnd = 0.0f;
};
