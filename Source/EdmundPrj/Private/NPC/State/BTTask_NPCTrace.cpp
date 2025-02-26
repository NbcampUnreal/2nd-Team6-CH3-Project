#include "NPC/State/BTTask_NPCTrace.h"
#include "NPC/NPC.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UBTTask_NPCTrace::UBTTask_NPCTrace()
{
	NodeName = TEXT("NPC Trace");
	bNotifyTick = true;
	NPCRef = nullptr;
	CachedOwnerComp = nullptr;
}

EBTNodeResult::Type UBTTask_NPCTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon)
	{
		return EBTNodeResult::Failed;
	}

	NPCRef = Cast<ANPC>(AICon->GetPawn());
	if (!NPCRef)
	{
		return EBTNodeResult::Failed;
	}

	CachedOwnerComp = &OwnerComp;
	return EBTNodeResult::InProgress;
}

void UBTTask_NPCTrace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!NPCRef)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AActor* Player = UGameplayStatics::GetPlayerPawn(NPCRef->GetWorld(), 0);
	if (!Player)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector Start = NPCRef->GetActorLocation() + FVector(0, 0, NPCRef->BaseEyeHeight);
	FVector End = Player->GetActorLocation();

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(NPCRef);

	bool bHit = NPCRef->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams);

	DrawDebugLine(NPCRef->GetWorld(), Start, End, FColor::Green, false, DeltaSeconds, 0, 1.0f);

	AAIController* AICon = Cast<AAIController>(NPCRef->GetController());
	if (!AICon)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (bHit && Hit.GetActor() == Player)
	{
		AICon->MoveToActor(Player, 5.0f);
	}
	else
	{
		AICon->MoveToLocation(End);
	}
}
