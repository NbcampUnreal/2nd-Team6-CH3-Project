#include "Boss/State/BTTask_BossAttack2.h"
#include "Boss/Boss.h"
#include "AIController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
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

	CachedOwnerComp = &OwnerComp;

	BossRef = Cast<ABoss>(AIController->GetPawn());
	if (!BossRef)
	{
		return EBTNodeResult::Failed;
	}

	BossRef->BTTask_BossAttack2Instance = this;
	UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
	if (AnimInst && AnimInst->Attack2Montage)
	{
		UAnimInstance* AnimInstance = BossRef->GetMesh()->GetAnimInstance();
		AnimInst->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UBTTask_BossAttack2::OnMontageNotifyBegin);
		AnimInstance->Montage_SetPlayRate(AnimInst->Attack2Montage, 0.1f);
		AnimInst->OnPlayMontageNotifyBegin.AddDynamic(this, &UBTTask_BossAttack2::OnMontageNotifyBegin);
		AnimInstance->Montage_Play(AnimInst->Attack2Montage);
	}
	StartAscend();

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
		if (BossRef->GetCharacterMovement()->MovementMode != MOVE_Flying)
		{
			BossRef->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		}
		FVector CurrentActorLocation = BossRef->GetActorLocation();
		FVector SocketLocation = BossRef->GetMesh()->GetSocketLocation(TEXT("Back_Left_FootCapsuleComponent"));
		float ActorSocketOffset = CurrentActorLocation.Z - SocketLocation.Z;

		float NewSocketZ = SocketLocation.Z + BossRef->Attack2_AscendSpeed * DeltaSeconds;

		if (NewSocketZ >= BossRef->Attack2_TargetHeight)
		{
			float TargetActorZ = BossRef->Attack2_TargetHeight + ActorSocketOffset;
			FVector NewActorLocation = CurrentActorLocation;
			NewActorLocation.Z = TargetActorZ;
			BossRef->SetActorLocation(NewActorLocation, false);
			OnAscendComplete();
		}
		else
		{
			FVector NewActorLocation = CurrentActorLocation;
			NewActorLocation.Z += BossRef->Attack2_AscendSpeed * DeltaSeconds;
			BossRef->SetActorLocation(NewActorLocation, false);
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
		FVector CurrentActorLocation = BossRef->GetActorLocation();
		FVector NewActorLocation = CurrentActorLocation - FVector(0, 0, BossRef->Attack2_DescendSpeed * DeltaSeconds);
		FVector SocketLocation = BossRef->GetMesh()->GetSocketLocation(TEXT("Back_Left_FootCapsuleComponent"));
		float ActorSocketOffset = CurrentActorLocation.Z - SocketLocation.Z;

		FHitResult HitResult;
		FVector TraceStart = SocketLocation;
		FVector TraceEnd = TraceStart - FVector(0, 0, 5000.f);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(BossRef);

		if (BossRef->GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			float GroundZ = HitResult.Location.Z;
			float NewSocketZ = (NewActorLocation.Z - ActorSocketOffset);
			if (NewSocketZ <= GroundZ)
			{
				if (BossRef->GameState)
				{
					BossRef->GameState->PlayMonsterSound(BossRef->CurrentAudioComp, BossRef->GetMonsterType(), ESoundType::Attack2);
				}
				float TargetActorZ = GroundZ + ActorSocketOffset;
				NewActorLocation.Z = TargetActorZ;
				BossRef->SetActorLocation(NewActorLocation, false);

				if (BossRef->GetCharacterMovement())
				{
					BossRef->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					BossRef->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				}

				BossRef->ActivateAttack2Collision();
				BossRef->GetWorld()->GetTimerManager().SetTimer(
					TimerHandle_DisableCollision, BossRef, &ABoss::DeactivateAttack2Collision, 0.5f, false);

				BossRef->UpdateAttackCooldown(2);
				BossRef->SetbChaseComplete(true);
				FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
				return;
			}
		}

		BossRef->SetActorLocation(NewActorLocation, false);
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
		BossRef->GetCharacterMovement()->NavAgentProps.bCanFly = true;
		BossRef->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BossRef->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}

	if (BossRef->GameState)
	{
		BossRef->GameState->PlayMonsterSound(BossRef->CurrentAudioComp, BossRef->GetMonsterType(), ESoundType::Attack2);
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

	if (BossRef)
	{
		BossRef->ActivateAttack2Collision();
	}
}

void UBTTask_BossAttack2::OnAttack2Completed()
{
	if (CachedOwnerComp)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_BossAttack2::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == TEXT("BossAttack2_1"))
	{
		UAnimInstance* AnimInstance = BossRef->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(AnimInstance);
			if (AnimInst && AnimInst->Attack2Montage)
			{
				AnimInstance->Montage_SetPlayRate(AnimInst->Attack2Montage, 1.0f);
			}
		}
		StartAscend();
	}
}

