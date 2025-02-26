#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

class ABoss;

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
    
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss")
    ABoss* BossCharacter;
      
private:
    float Attack1_CooldownEnd = 0.0f;
    float Attack2_CooldownEnd = 0.0f;
    float Attack3_CooldownEnd = 0.0f;
    float Attack4_CooldownEnd = 0.0f;
};
