#include "Player/PlayerCharacterAurora.h"
#include "Player/EdmundPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerCharacterAurora::APlayerCharacterAurora()
{
	PrimaryActorTick.bCanEverTick = false;

	AttackSound = nullptr;

	IsAttack = false;
	ComboCount = 0;
	ResetDelay = 1.0f;
	AttackRangeRadius = 300.0f;
}

void APlayerCharacterAurora::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacterAurora::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AEdmundPlayerController* PlayerController = Cast<AEdmundPlayerController>(GetController()))
		{
			if (PlayerController->AttackAction)
			{
				EnhancedInput->BindAction(
					PlayerController->AttackAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacterAurora::Attack
				);
			}
		}
	}
}

void APlayerCharacterAurora::Attack(const FInputActionValue& value)
{
	if (!IsAttack)
	{
		IsAttack = true;

		switch (ComboCount)
		{
			case 0:
				if (IsValid(AttackMontages[0]))
				{
					PlayAnimMontage(AttackMontages[0]);
				}
				break;

			case 1:
				if (IsValid(AttackMontages[1]))
				{
					PlayAnimMontage(AttackMontages[1]);
				}
				break;

			case 2:
				if (IsValid(AttackMontages[2]))
				{
					PlayAnimMontage(AttackMontages[2]);
				}
				break;

			case 3:
				if (IsValid(AttackMontages[3]))
				{
					PlayAnimMontage(AttackMontages[3]);
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

	FVector Start = GetActorLocation() + (ForwardVector * AttackRangeRadius); // ���� ���� ��ġ
	FVector End = Start + (ForwardVector * AttackRangeRadius); // ���� �� ��ġ

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
					30.0f,	// �����ʿ�
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
