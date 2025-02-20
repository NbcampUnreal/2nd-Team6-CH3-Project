#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BossAIController.generated.h"

UCLASS()
class EDMUNDPRJ_API ABossAIController : public AAIController
{
    GENERATED_BODY()

public:
    ABossAIController();

protected:
    virtual void BeginPlay() override;

public:
    void SetBehaviorTree(UBehaviorTree* NewBehaviorTree);

private:
    UPROPERTY(VisibleAnywhere, Category = "AI")
    UBehaviorTreeComponent* BehaviorTreeComponent;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    UBlackboardComponent* BlackboardComponent;

    UPROPERTY(EditAnywhere, Category = "AI")
    UBehaviorTree* BehaviorTree;
};