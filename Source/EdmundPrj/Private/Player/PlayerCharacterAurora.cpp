#include "Player/PlayerCharacterAurora.h"
#include "Player/EdmundPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/EdmundGameState.h"

APlayerCharacterAurora::APlayerCharacterAurora()
{
	PrimaryActorTick.bCanEverTick = false;

	IsAttack = false;
	ComboCount = 0;
	ResetDelay = 1.0f;
	AttackRangeRadius = 150.0f;
	ComboTimeDuration = 1.2f;
}

void APlayerCharacterAurora::BeginPlay()
{
	Super::BeginPlay();

	ComboTimeDuration = ComboTimeDuration - AttackDelay;
}

void APlayerCharacterAurora::Attack(const FInputActionValue& value)
{

	if (!IsAttack)
	{
		IsAttack = true;

		Super::Attack(value);

		switch (ComboCount)
		{
			case 0:
				if (IsValid(AttackMontages[0]))
				{
					if (IsValid(CurrentGameState))
					{
						CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Attack);
					}

					PlayAnimMontage(AttackMontages[0], ComboTimeDuration);
				}

				break;

			case 1:
				if (IsValid(AttackMontages[1]))
				{
					if (IsValid(CurrentGameState))
					{
						CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Attack2);
					}

					PlayAnimMontage(AttackMontages[1], ComboTimeDuration);
				}

				break;

			case 2:
				if (IsValid(AttackMontages[2]))
				{
					if (IsValid(CurrentGameState))
					{
						CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Attack3);
					}

					PlayAnimMontage(AttackMontages[2], ComboTimeDuration);
				}

				break;

			case 3:
				if (IsValid(AttackMontages[3]))
				{
					if (IsValid(CurrentGameState))
					{
						CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Attack4);
					}

					PlayAnimMontage(AttackMontages[3], ComboTimeDuration);
				}

				break;

			default:
				break;
		}

		// 콤보 공격 딜레이
		GetWorld()->GetTimerManager().SetTimer(
			ComboResetTimerHandle,
			this,
			&APlayerCharacterAurora::ResetCombo,
			ResetDelay,
			false
		);
	}
}

void APlayerCharacterAurora::AttackTrace()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FRotator ControlRotation = PlayerController->GetControlRotation();
	FVector ForwardVector = ControlRotation.Vector();

	FVector Start = GetActorLocation() + (ForwardVector * (AttackRangeRadius + 10)); // 공격 시작 위치
	FVector End = Start + (ForwardVector * (AttackRangeRadius + 10)); // 공격 끝 위치

	TArray<FHitResult> HitResults;

	// 트레이스 수행
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 자신은 무시하도록 설정

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,               // 시작 위치
		End,                 // 끝 위치
		FQuat::Identity,     // 회전값 (회전 없이)
		ECollisionChannel::ECC_OverlapAll_Deprecated, // 충돌 채널
		FCollisionShape::MakeSphere(AttackRangeRadius), // 범위 설정 (구체 모양)
		QueryParams
	);

	// 데미지를 입힌 액터를 추적할 Set (중복 방지)
	// Set이 없으면 근접공격한번에 여러번 데미지 받는 현상 발생
	TSet<AActor*> DamagedActors;

	if (bHit)
	{
		bool IsBossAttack = false;

		// 여러 충돌 객체가 있다면
		for (const FHitResult& Hit : HitResults)
		{
			// 충돌한 객체가 있다면
			AActor* HitActor = Hit.GetActor();

			if (!DamagedActors.Contains(HitActor) && HitActor && (HitActor->ActorHasTag("MissionItem") || HitActor->ActorHasTag("Monster")))
			{
				if (!IsBossAttack && HitActor->ActorHasTag("Boss"))
				{
					if (IsBossAttack)
					{
						continue;
					}

					IsBossAttack = true;
				}

				UGameplayStatics::ApplyDamage(
					HitActor,
					GetAttackDamage(),
					nullptr,
					this,
					UDamageType::StaticClass()
				);
			}

			// 데미지를 입힌 액터를 Set에 추가
			DamagedActors.Add(HitActor);
		}
	}
}

void APlayerCharacterAurora::NextCombo()
{
	IsAttack = false;

	ComboCount = ++ComboCount % 4;
}

void APlayerCharacterAurora::ResetCombo()
{
	ComboCount = 0;
}
