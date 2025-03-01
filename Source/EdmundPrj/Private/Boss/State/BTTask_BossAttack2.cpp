#include "Boss/State/BTTask_BossAttack2.h"
#include "Boss/Boss.h"
#include "AIController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/Boss_AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_BossAttack2::UBTTask_BossAttack2()
{
	NodeName = TEXT("Boss Attack2");
	bNotifyTick = true;
	BossRef = nullptr;
	CachedOwnerComp = nullptr;
	CurrentPhase = 0;
}

EBTNodeResult::Type UBTTask_BossAttack2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	CurrentPhase = 0;

	BossRef->GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_Phase, this, &UBTTask_BossAttack2::StartAscend, 2.0f, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_BossAttack2::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!BossRef)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	switch (CurrentPhase)
	{
	case 1:
	{
		FVector CurrentLocation = BossRef->GetActorLocation();
		FVector NewLocation = CurrentLocation + FVector(0, 0, BossRef->Attack2_AscendSpeed * DeltaSeconds);
		if (NewLocation.Z >= BossRef->Attack2_TargetHeight)
		{
			NewLocation.Z = BossRef->Attack2_TargetHeight;
			BossRef->SetActorLocation(NewLocation, false);
			OnAscendComplete();
		}
		else
		{
			BossRef->SetActorLocation(NewLocation, false);
		}
		break;
	}
	case 3:
	{
		FVector CurrentLocation = BossRef->GetActorLocation();
		BossRef->SetActorLocation(FVector(TargetHorizontalLocation.X, TargetHorizontalLocation.Y, CurrentLocation.Z), false);
		CurrentPhase = 4;
		BossRef->GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_Phase, this, &UBTTask_BossAttack2::StartDescend, 1.0f, false);
		break;
	}
	case 5:
	{
		FVector CurrentLocation = BossRef->GetActorLocation();
		FVector NewLocation = CurrentLocation - FVector(0, 0, BossRef->Attack2_DescendSpeed * DeltaSeconds);
		FHitResult HitResult;
		FVector TraceStart = CurrentLocation;
		FVector TraceEnd = CurrentLocation - FVector(0, 0, 20000.0f); 
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(BossRef);

		if (BossRef->GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			float GroundZ = HitResult.Location.Z;
			if (NewLocation.Z <= GroundZ)
			{
				NewLocation.Z = GroundZ;
				BossRef->SetActorLocation(NewLocation, false);
				FRotator LandingRotation = UKismetMathLibrary::MakeRotFromZX(HitResult.Normal, BossRef->GetActorForwardVector());
				LandingRotation.Pitch = 0.0f;
				BossRef->SetActorRotation(LandingRotation);

				if (BossRef->GetCharacterMovement())
				{
					BossRef->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				}
				BossRef->UpdateAttackCooldown(2);
				BossRef->SetbChaseComplete(true);
				FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
			}
			else
			{
				BossRef->SetActorLocation(NewLocation, false);
			}
		}
		break;
	}

	default:
		break;
	}
}

void UBTTask_BossAttack2::StartAscend()
{
	if (!BossRef) return;

	if (BossRef->GetCharacterMovement())
	{
		BossRef->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
	CurrentPhase = 1;
	DelayedFire_Attack2();
}

void UBTTask_BossAttack2::OnAscendComplete()
{
	CurrentPhase = 2;
	BossRef->GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_Phase, [this]()
		{
			AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
			if (Player)
			{
				FVector PlayerLocation = Player->GetActorLocation();
				FVector CurrentLocation = BossRef->GetActorLocation();
				TargetHorizontalLocation = FVector(PlayerLocation.X, PlayerLocation.Y, CurrentLocation.Z);
			}
			else
			{
				TargetHorizontalLocation = BossRef->GetActorLocation();
			}
			CurrentPhase = 3;
		}, 1.0f, false);
}

void UBTTask_BossAttack2::DelayedFire_Attack2()
{
	if (!BossRef) return;

	UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
	float Duration = 2.0f;
	if (AnimInst && AnimInst->Attack2Montage)
	{
		Duration = BossRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimInst->Attack2Montage);
		BossRef->GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_Phase, this, &UBTTask_BossAttack2::DelayedTransition, Duration, false);
	}
	else
	{
		BossRef->GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_Phase, this, &UBTTask_BossAttack2::DelayedTransition, 2.0f, false);
	}
}

void UBTTask_BossAttack2::DelayedTransition()
{
	if (CurrentPhase < 3)
	{
		CurrentPhase = 2;
	}
}

void UBTTask_BossAttack2::StartDescend()
{
	CurrentPhase = 5;
}
