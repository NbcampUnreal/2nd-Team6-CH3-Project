#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API ABossAIController : public AAIController {
    GENERATED_BODY()

private:
    ABoss* BossCharacter;

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
};
