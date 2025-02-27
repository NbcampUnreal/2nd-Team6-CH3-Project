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

	CachedOwnerComp = &OwnerComp;
	BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Fire, this, &UBTTask_BossAttack1::DelayedFire, 2.0f, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_BossAttack1::DelayedFire()
{
	if (!BossRef) return;

	UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
	float TransitionDelay = 1.0f;
	if (AnimInst && AnimInst->Attack1Montage)
	{
		float Duration = BossRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimInst->Attack1Montage);
		TransitionDelay = Duration;
	}
	else
	{
		TransitionDelay = 1.0f;
	}
	BossRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle_Transition, this, &UBTTask_BossAttack1::DelayedTransition, TransitionDelay, false);
	AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
	if (Player)
	{
		FVector PlayerLocation = Player->GetActorLocation();
		FVector BossLocation = BossRef->GetActorLocation();
		FVector Direction = (PlayerLocation - BossLocation).GetSafeNormal();
		FRotator TargetRotation = Direction.Rotation();
		BossRef->SetActorRotation(TargetRotation);
	}
	FireBullet();
}

void UBTTask_BossAttack1::DelayedTransition()
{
	if (!BossRef || !CachedOwnerComp)
	{
		return;
	}
	BossRef->UpdateAttackCooldown(1);
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}

void UBTTask_BossAttack1::FireBullet()
{
	if (!BossRef || !BossRef->GetWorld() || !BossRef->MuzzleLocation)
	{
		return;
	}

	if (!BossRef->Attack1BulletClass)
	{
		return;
	}

	FVector SpawnLocation = BossRef->MuzzleLocation->GetComponentLocation();

	AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
	if (!Player)
	{
		return;
	}

	FVector PlayerLocation = Player->GetActorLocation();
	FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();
	FRotator TargetRotation = Direction.Rotation();
	FRotator NewRotation = FMath::RInterpTo(BossRef->GetActorRotation(), TargetRotation, BossRef->GetWorld()->GetDeltaSeconds(), 5.0f);
	BossRef->SetActorRotation(NewRotation);
	ABoss_Attack1_Bullet* Bullet = ABoss_Attack1_Bullet::GetBulletFromPool(BossRef->GetWorld(), BossRef->Attack1BulletClass, BossRef);
	if (Bullet)
	{
		Bullet->FireProjectile(SpawnLocation, TargetRotation, Direction);
	}
}
