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

	// �������� ���� ���͸� ������ Set (�ߺ� ����)
	// Set�� ������ ���������ѹ��� ������ ������ �޴� ���� �߻�
	TSet<AActor*> DamagedActors;

	if (bHit)
	{
		bool IsBossAttack = false;

		// ���� �浹 ��ü�� �ִٸ�
		for (const FHitResult& Hit : HitResults)
		{
			// �浹�� ��ü�� �ִٸ�
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
