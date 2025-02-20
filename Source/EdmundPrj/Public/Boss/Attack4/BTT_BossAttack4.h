#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossAttack4.generated.h"

UCLASS()
class EDMUNDPRJ_API UBTT_BossAttack4 : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_BossAttack4();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UPROPERTY(EditAnywhere, Category = "Bullet")
    TSubclassOf<AActor> BulletClass;

    UPROPERTY(EditAnywhere, Category = "Bullet")
    int32 BulletCount = 5; // ÅºÈ¯ °³¼ö

    UPROPERTY(EditAnywhere, Category = "Bullet")
    float BulletSpawnRadius = 200.0f; // ÅºÈ¯ ½ºÆù ¹Ý°æ

    FTimerHandle AttackDelayTimerHandle;

    void SpawnBullets(AActor* BossActor);
};
