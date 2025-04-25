#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NPCAIController.generated.h"

class ANPC;

UCLASS()
class EDMUNDPRJ_API ANPCAIController : public AAIController
{
    GENERATED_BODY()

public:
    ANPCAIController();
    virtual void OnPossess(APawn* InPawn) override;
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    ANPC* NPCCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;
};
