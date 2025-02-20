#include "Boss/Attack4/BTT_BossAttack4.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Boss/Attack4/BossAttack4Bullet.h"

UBTT_BossAttack4::UBTT_BossAttack4()
{
    NodeName = "Boss Attack 4";
}

EBTNodeResult::Type UBTT_BossAttack4::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        return EBTNodeResult::Failed;
    }

    AActor* BossActor = AICon->GetPawn();
    if (!BossActor)
    {
        return EBTNodeResult::Failed;
    }

    // 3초 동안 정지 후 탄환 생성
    GetWorld()->GetTimerManager().SetTimer(AttackDelayTimerHandle, FTimerDelegate::CreateLambda([this, BossActor]()
        {
            SpawnBullets(BossActor);
        }), 3.0f, false);

    return EBTNodeResult::Succeeded;
}

void UBTT_BossAttack4::SpawnBullets(AActor* BossActor)
{
    if (!BossActor)
    {
        return;
    }

    if (!BulletClass)
    {
        return;
    }

    if (!GetWorld())
    {
        return;
    }

    for (int32 i = 0; i < BulletCount; i++)
    {
        float Angle = i * (360.0f / BulletCount);
        FVector SpawnOffset = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)), FMath::Sin(FMath::DegreesToRadians(Angle)), 0) * BulletSpawnRadius;
        FVector SpawnLocation = BossActor->GetActorLocation() + SpawnOffset;

        AActor* SpawnedBullet = GetWorld()->SpawnActor<AActor>(BulletClass, SpawnLocation, FRotator::ZeroRotator);

    }
}


