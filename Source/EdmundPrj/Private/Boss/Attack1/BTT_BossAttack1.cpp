#include "Boss/Attack1/BTT_BossAttack1.h"
#include "Boss/Attack1/BossAttack1Bullet.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_BossAttack1::UBTT_BossAttack1()
{
    NodeName = "Boss Attack 1";
}

EBTNodeResult::Type UBTT_BossAttack1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        return EBTNodeResult::Failed;
    }

    APawn* BossPawn = AICon->GetPawn();
    if (!BossPawn)
    {
        return EBTNodeResult::Failed;
    }

    AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Player"));
    if (!Player)
    {
        return EBTNodeResult::Failed;
    }

    // ÅºÈ¯ À§Ä¡
    FVector SpawnLocation = BossPawn->GetActorLocation() + FVector(0, 0, 50.0f);
    FRotator SpawnRotation = (Player->GetActorLocation() - SpawnLocation).Rotation();

    // ÅºÈ¯ »ý¼º
    FActorSpawnParameters SpawnParams;
    ABossAttack1Bullet* Bullet = BossPawn->GetWorld()->SpawnActor<ABossAttack1Bullet>(
        ABossAttack1Bullet::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

    if (Bullet)
    {
        Bullet->Initialize(Player->GetActorLocation());
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
