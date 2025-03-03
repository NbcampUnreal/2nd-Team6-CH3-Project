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
					if (IsValid(AttackSounds[0]))
					{
						CurrentAudioComp->SetSound(AttackSounds[0]);
						CurrentAudioComp->Play();
					}

					if (IsValid(WeaponSounds[0]))
					{
						UAudioComponent* WeaponAudioComp = UGameplayStatics::SpawnSoundAtLocation(this, WeaponSounds[0], GetActorLocation());
						WeaponAudioComp->Play();
					}

					PlayAnimMontage(AttackMontages[0], ComboTimeDuration);
				}

				break;

			case 1:
				if (IsValid(AttackMontages[1]))
				{

					if (IsValid(AttackSounds[1]))
					{
						CurrentAudioComp->SetSound(AttackSounds[1]);
						CurrentAudioComp->Play();
					}

					if (IsValid(WeaponSounds[1]))
					{
						UAudioComponent* WeaponAudioComp = UGameplayStatics::SpawnSoundAtLocation(this, WeaponSounds[1], GetActorLocation());
						WeaponAudioComp->Play();
					}

					PlayAnimMontage(AttackMontages[1], ComboTimeDuration);
				}

				break;

			case 2:
				if (IsValid(AttackMontages[2]))
				{
					if (IsValid(AttackSounds[2]))
					{
						CurrentAudioComp->SetSound(AttackSounds[2]);
						CurrentAudioComp->Play();
					}

					if (IsValid(WeaponSounds[2]))
					{
						UAudioComponent* WeaponAudioComp = UGameplayStatics::SpawnSoundAtLocation(this, WeaponSounds[2], GetActorLocation());
						WeaponAudioComp->Play();
					}

					PlayAnimMontage(AttackMontages[2], ComboTimeDuration);
				}

				break;

			case 3:
				if (IsValid(AttackMontages[3]))
				{
					if (IsValid(AttackSounds[3]))
					{
						CurrentAudioComp->SetSound(AttackSounds[3]);
						CurrentAudioComp->Play();
					}

					if (IsValid(WeaponSounds[3]))
					{
						UAudioComponent* WeaponAudioComp = UGameplayStatics::SpawnSoundAtLocation(this, WeaponSounds[3], GetActorLocation());
						WeaponAudioComp->Play();
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

float APlayerCharacterAurora::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDie)
	{
		return 0.0f;
	}

	int32 DamageProb = FMath::RandRange(1, 100);

	// 회피 성공
	if (DamageProb <= EvasionProb)
	{
		// 회피 성공 사운드
		if (IsValid(EvasionSuccessSound))
		{
			CurrentAudioComp->SetSound(EvasionSuccessSound);
			CurrentAudioComp->Play();
		}

		return 0.0f;
	}

	// 공격 중에 피격 애니메이션 실행하면 다음 공격 못함
	if (IsValid(HitActionMontage) && !CheckAction() && !IsAttack)
	{

		PlayAnimMontage(HitActionMontage);
	}

	float ActualDamage = Super::Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 방어력 적용
	ActualDamage = (100 - Defense) * ActualDamage / 100;

	// HP는 정수, 데미지는 소수?
	// HP 음수 방지
	HP = FMath::Max(0.0f, HP - ActualDamage);

	if (HP == 0 && !IsDie)
	{
		// 부활 횟수가 있다면
		if (RevivalCount >= 1)
		{
			RevivalCount--;
			HP = MaxHP;

			// 부활 사운드
			if (IsValid(RevivalSuccessSound))
			{
				CurrentAudioComp->SetSound(RevivalSuccessSound);
				CurrentAudioComp->Play();
			}
		}
		// 부활 횟수가 없다면
		else
		{
			IsDie = true;
			ActiveDieAction();
		}
	}

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerHp(MaxHP, HP);
	}

	return ActualDamage;
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

	// 구체 보이게 하기
	if (GEngine)
	{
		DrawDebugSphere(
			GetWorld(),
			Start,
			AttackRangeRadius,
			12,
			FColor::Red,        // 색상
			false,              // 지속성 (게임 중 계속 표시할지 여부)
			1.0f                // 지속 시간
		);
	}

	// 데미지를 입힌 액터를 추적할 Set (중복 방지)
	// Set이 없으면 근접공격한번에 여러번 데미지 받는 현상 발생
	TSet<AActor*> DamagedActors;

	if (bHit)
	{
		// 여러 충돌 객체가 있다면
		for (const FHitResult& Hit : HitResults)
		{
			// 충돌한 객체가 있다면
			AActor* HitActor = Hit.GetActor();

			if (!DamagedActors.Contains(HitActor) && HitActor && (HitActor->ActorHasTag("MissionItem") || HitActor->ActorHasTag("Monster")))
			{
				UGameplayStatics::ApplyDamage(
					HitActor,
					30.0f,
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
