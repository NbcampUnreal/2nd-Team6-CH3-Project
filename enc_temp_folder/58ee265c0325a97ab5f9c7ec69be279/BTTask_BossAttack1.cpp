#include "Boss/State/BTTask_BossAttack1.h"
#include "Boss/Boss.h"
#include "AIController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Boss/Boss_AnimInstance.h"

UBTTask_BossAttack1::UBTTask_BossAttack1()
{
	NodeName = TEXT("Boss Attack1");
	BossRef = nullptr;
	CachedOwnerComp = nullptr;
}

EBTNodeResult::Type UBTTask_BossAttack1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	if (BossRef)
	{
		BossRef->FireBullet();
	}

	CachedOwnerComp = &OwnerComp;

	UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
	if (AnimInst && AnimInst->Attack1Montage)
	{
		float Duration = BossRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimInst->Attack1Montage);
		if (Duration > 0.0f)
		{
			FTimerHandle MontageEndTimer;
			BossRef->GetWorldTimerManager().SetTimer(MontageEndTimer, this, &UBTTask_BossAttack1::OnAnimationCompleted, Duration, false);
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_BossAttack1::OnAnimationCompleted()
{
	if (!BossRef || !CachedOwnerComp)
	{
		return;
	}

	BossRef->UpdateAttackCooldown(1);
	BossRef->SetbChaseComplete(true);

	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}