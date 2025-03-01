#include "Boss/State/BTTask_BossChase.h"
#include "Boss/Boss.h"
#include "AIController.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	BossRef->GetCharacterMovement()->MaxWalkSpeed = BossRef->GetMonsterMoveSpeed();
	AccumulatedTime += DeltaSeconds;
	if (AccumulatedTime >= FMath::RandRange(2.0f, 3.0f)) // 랜덤 2~3초 지속
	{
		BossRef->SetbChaseComplete(false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	AAIController* AICon = Cast<AAIController>(BossRef->GetController());
	AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
	if (!AICon || !Player)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	float Distance = FVector::Dist(BossRef->GetActorLocation(), Player->GetActorLocation());
	if (Distance >= 1500.0f)
	{
		BossRef->Chase_AcceptanceRadius += 100.0f;
	}

	AICon->MoveToActor(Player, BossRef->Chase_AcceptanceRadius);
	BossRef->SetbChaseComplete(false);

	int32 NextAttack = 0;
	if (UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent())
	{
		NextAttack = BBComp->GetValueAsInt("NextAttack");
	}

	if (NextAttack != 0 || Distance <= BossRef->Chase_AcceptanceRadius + 500.0f)
	{
		BossRef->SetbChaseComplete(true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
