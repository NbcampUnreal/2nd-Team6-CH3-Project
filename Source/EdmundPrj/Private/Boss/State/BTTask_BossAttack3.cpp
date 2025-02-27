#include "Boss/State/BTTask_BossAttack3.h"
#include "Boss/Boss.h"
#include "AIController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/Boss_AnimInstance.h"
#include "Boss/Attack/Boss_Attack1_Bullet.h"

UBTTask_BossAttack3::UBTTask_BossAttack3()
{
	NodeName = TEXT("Boss Attack3");
	bNotifyTick = true;
	BossRef = nullptr;
	CachedOwnerComp = nullptr;
	ComboPhase = 0;
	RangedComboBulletCount = 0;
	CurrentPhaseTimer = 0.0f;
}

EBTNodeResult::Type UBTTask_BossAttack3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	ComboPhase = 0;
	RangedComboBulletCount = 0;
	CurrentPhaseTimer = 0.0f;
	return EBTNodeResult::InProgress;
}

void UBTTask_BossAttack3::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!BossRef)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	CurrentPhaseTimer += DeltaSeconds;

	switch (ComboPhase)
	{
	case 0:
	{
		if (CurrentPhaseTimer >= InitialDelay)
		{
			CurrentPhaseTimer = 0.0f;
			ComboPhase = 1;
			ExecuteMeleeAttack();
		}
		break;
	}
	case 1:
	case 2:
	{
		if (CurrentPhaseTimer >= MeleeAttackInterval)
		{
			CurrentPhaseTimer = 0.0f;
			ExecuteMeleeAttack();
			if (ComboPhase >= 3)
			{
				CurrentPhaseTimer = 0.0f;
			}
		}
		break;
	}
	case 3:
	{
		if (CurrentPhaseTimer >= RangedAttackInterval)
		{
			CurrentPhaseTimer = 0.0f;
			ExecuteRangedComboAttack();
			RangedComboBulletCount++;
			if (RangedComboBulletCount >= 5)
			{
				FinishComboAttack();
				return;
			}
		}
		break;
	}
	default:
		break;
	}
}

void UBTTask_BossAttack3::ExecuteMeleeAttack()
{
	if (!BossRef)
		return;

	float DashDistance = (ComboPhase == 1) ? 200.0f : 300.0f;
	FVector DashDirection = BossRef->GetActorForwardVector();
	FVector CurrentLocation = BossRef->GetActorLocation();
	FVector NewLocation = CurrentLocation + DashDirection * DashDistance;
	BossRef->SetActorLocation(NewLocation, true);
	BossRef->MonsterAttackCheck();
	if (ComboPhase == 1)
	{
		PlayAttack3Montage();
	}
	ComboPhase++;
}

void UBTTask_BossAttack3::ExecuteRangedComboAttack()
{
	if (!BossRef || !BossRef->Attack1BulletClass)
		return;

	FVector SpawnLocation = BossRef->MuzzleLocation->GetComponentLocation();
	AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
	if (!Player)
		return;

	FVector PlayerLocation = Player->GetActorLocation();
	FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();
	FRotator TargetRotation = Direction.Rotation();

	ABoss_Attack1_Bullet* Bullet = ABoss_Attack1_Bullet::GetBulletFromPool(BossRef->GetWorld(), BossRef->Attack1BulletClass, BossRef);
	if (Bullet)
	{
		Bullet->FireProjectile(SpawnLocation, TargetRotation, Direction);
	}
}

void UBTTask_BossAttack3::FinishComboAttack()
{
	if (!BossRef)
	{
		return;
	}
	BossRef->UpdateAttackCooldown(3);
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}

void UBTTask_BossAttack3::PlayAttack3Montage()
{
	if (!BossRef)
	{
		return;
	}
	UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
	if (AnimInst && AnimInst->Attack3Montage)
	{
		BossRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimInst->Attack3Montage);
	}
}
