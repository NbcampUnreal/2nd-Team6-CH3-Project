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

		// �޺� ���� ������
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

	// ȸ�� ����
	if (DamageProb <= EvasionProb)
	{
		// ȸ�� ���� ����
		if (IsValid(EvasionSuccessSound))
		{
			CurrentAudioComp->SetSound(EvasionSuccessSound);
			CurrentAudioComp->Play();
		}

		return 0.0f;
	}

	// ���� �߿� �ǰ� �ִϸ��̼� �����ϸ� ���� ���� ����
	if (IsValid(HitActionMontage) && !CheckAction() && !IsAttack)
	{

		PlayAnimMontage(HitActionMontage);
	}

	float ActualDamage = Super::Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// ���� ����
	ActualDamage = (100 - Defense) * ActualDamage / 100;

	// HP�� ����, �������� �Ҽ�?
	// HP ���� ����
	HP = FMath::Max(0.0f, HP - ActualDamage);

	if (HP == 0 && !IsDie)
	{
		// ��Ȱ Ƚ���� �ִٸ�
		if (RevivalCount >= 1)
		{
			RevivalCount--;
			HP = MaxHP;

			// ��Ȱ ����
			if (IsValid(RevivalSuccessSound))
			{
				CurrentAudioComp->SetSound(RevivalSuccessSound);
				CurrentAudioComp->Play();
			}
		}
		// ��Ȱ Ƚ���� ���ٸ�
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

	FVector Start = GetActorLocation() + (ForwardVector * (AttackRangeRadius + 10)); // ���� ���� ��ġ
	FVector End = Start + (ForwardVector * (AttackRangeRadius + 10)); // ���� �� ��ġ

	TArray<FHitResult> HitResults;

	// Ʈ���̽� ����
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // �ڽ��� �����ϵ��� ����

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,               // ���� ��ġ
		End,                 // �� ��ġ
		FQuat::Identity,     // ȸ���� (ȸ�� ����)
		ECollisionChannel::ECC_OverlapAll_Deprecated, // �浹 ä��
		FCollisionShape::MakeSphere(AttackRangeRadius), // ���� ���� (��ü ���)
		QueryParams
	);

	// ��ü ���̰� �ϱ�
	if (GEngine)
	{
		DrawDebugSphere(
			GetWorld(),
			Start,
			AttackRangeRadius,
			12,
			FColor::Red,        // ����
			false,              // ���Ӽ� (���� �� ��� ǥ������ ����)
			1.0f                // ���� �ð�
		);
	}

	// �������� ���� ���͸� ������ Set (�ߺ� ����)
	// Set�� ������ ���������ѹ��� ������ ������ �޴� ���� �߻�
	TSet<AActor*> DamagedActors;

	if (bHit)
	{
		// ���� �浹 ��ü�� �ִٸ�
		for (const FHitResult& Hit : HitResults)
		{
			// �浹�� ��ü�� �ִٸ�
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

			// �������� ���� ���͸� Set�� �߰�
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
