#include "Boss/State/BTTask_BossChase.h"
#include "Boss/Boss.h"
#include "AIController.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_BossChase::UBTTask_BossChase()
{
	NodeName = TEXT("Boss Chase");
	bNotifyTick = true;
	BossRef = nullptr;
	CachedOwnerComp = nullptr;
	AccumulatedTime = 0.0f;
	
}

EBTNodeResult::Type UBTTask_BossChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	BossRef = Cast<ABoss>(AIController->GetPawn());
	if (!BossRef)
	{
		return EBTNodeResult::Failed;
	}

	CachedOwnerComp = &OwnerComp;
	AccumulatedTime = 0.0f;
	return EBTNodeResult::InProgress;
}

void UBTTask_BossChase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (!BossRef)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    AccumulatedTime += DeltaSeconds;
    if (AccumulatedTime >= 0.5f)
    {
        AccumulatedTime = 0.0f;
        AAIController* AICon = Cast<AAIController>(BossRef->GetController());
        AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
        if (!AICon || !Player)
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            return;
        }
        AICon->MoveToActor(Player, BossRef->Chase_AcceptanceRadius);

        int32 NextAttack = 0;
        if (UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent())
        {
            NextAttack = BBComp->GetValueAsInt("NextAttack");
        }

        if (NextAttack != 0)
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
        /*
        else
        {
            float Distance = FVector::Dist(BossRef->GetActorLocation(), Player->GetActorLocation());
            if (Distance <= BossRef->Chase_AcceptanceRadius)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            }
        }
        */
    }
}
