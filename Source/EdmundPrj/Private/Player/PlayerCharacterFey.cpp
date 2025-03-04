#include "Player/PlayerCharacterFey.h"
#include "EnhancedInputComponent.h"
#include "Player/EdmundPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "System/EdmundGameState.h"

APlayerCharacterFey::APlayerCharacterFey()
{
	PrimaryActorTick.bCanEverTick = false;

	IsHover = false;
	HoveringDelay = 0.1f;

	JumpCount = 0;
	DoubleJumpHeight = 600.0f;
	HoveringZSpeed = -150.0f;

	MeleeAttackDelay = 0.7f;
	MeleeAttackRadius = 100.0f;
	MeleeAttackPushStrength = 1000.0f;

	AttackMontage = nullptr;

	IsAttack = false;
	IsMeleeAttack = false;

	CanCrouchCharacter = false;
}

void APlayerCharacterFey::BeginPlay()
{
	Super::BeginPlay();

	AttackDelay *= 2;

	WeaponActor = GetWorld()->SpawnActor<AWeapon>(Weapon);

	if (Weapon)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		WeaponActor->AttachToComponent(GetMesh(), TransformRules, TEXT("WeaponSocket"));
		WeaponActor->SetOwner(this);
	}
}

void APlayerCharacterFey::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AEdmundPlayerController* PlayerController = Cast<AEdmundPlayerController>(GetController()))
		{
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacterFey::TriggerJump
				);
			}
			if (PlayerController->MeleeAttackAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MeleeAttackAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacterFey::MeleeAttack
				);
			}
		}
	}
}

void APlayerCharacterFey::StartJump(const FInputActionValue& value)
{
	// ù ��° ������ �����ϸ�
	if (CanJump())
	{
		Super::Jump(); // ù ��° ���� ����
		JumpCount = 1; // ���� �� 2�� ���� ����
	}
	// ù ��° ���� ��, �������� ���¿��� �� ��° ������ ����
	else if (JumpCount < 2) // �̹� ���߿� ���� �� (IsFalling()�� true)
	{
		// 2�� ���� ����
		FVector JumpVelocity = GetVelocity();

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		bool IsMove = false;

		// Ư�� Ű(W Ű)�� ���ȴ��� Ȯ��
		if (PlayerController && PlayerController->IsInputKeyDown(EKeys::W))
		{
			IsMove = true;
			JumpVelocity = GetActorForwardVector() * GetCharacterMovement()->MaxWalkSpeed;
		}

		// Ư�� Ű(S Ű)�� ���ȴ��� Ȯ��
		else if (PlayerController && PlayerController->IsInputKeyDown(EKeys::S))
		{
			IsMove = true;
			JumpVelocity = GetActorForwardVector() * -1.0f * GetCharacterMovement()->MaxWalkSpeed;
		}

		// Ư�� Ű(A Ű)�� ���ȴ��� Ȯ��
		if (PlayerController && PlayerController->IsInputKeyDown(EKeys::A))
		{
			if (!IsMove)
			{
				JumpVelocity = GetActorRightVector() * -1.0f * GetCharacterMovement()->MaxWalkSpeed;
			}
			else
			{
				JumpVelocity = JumpVelocity + GetActorRightVector() * -1.0f * GetCharacterMovement()->MaxWalkSpeed;
			}
		}

		// Ư�� Ű(D Ű)�� ���ȴ��� Ȯ��
		else if (PlayerController && PlayerController->IsInputKeyDown(EKeys::D))
		{
			if (!IsMove)
			{
				JumpVelocity = GetActorRightVector() * 1.0f * GetCharacterMovement()->MaxWalkSpeed;
			}
			else
			{
				JumpVelocity = JumpVelocity + GetActorRightVector() * 1.0f * GetCharacterMovement()->MaxWalkSpeed;
			}
		}

		JumpVelocity.Z = DoubleJumpHeight;

		GetCharacterMovement()->Launch(JumpVelocity);  // Z�� �������� ����

		// 2�� ���� ��, 2�� ���� �Ұ����ϰ� ����
		JumpCount++;

		IsHover = true;

		GetWorld()->GetTimerManager().SetTimer(
			HoveringDelayHandle,
			this,
			&APlayerCharacterFey::SetIsHover,
			0.1f,
			false
		);
	}
}

void APlayerCharacterFey::TriggerJump(const FInputActionValue& value)
{
	UCharacterMovementComponent* CharacterMovement2 = GetCharacterMovement();

	FVector CurrentVelocity = GetVelocity();
	if (CurrentVelocity.Z < HoveringZSpeed && !IsHover)
	{
		CurrentVelocity.Z = HoveringZSpeed;		// �ϰ� �ӵ� ����
		GetCharacterMovement()->Launch(CurrentVelocity);
	}
}

void APlayerCharacterFey::MeleeAttack(const FInputActionValue& value)
{
	if (CheckAction() || IsDie)
	{
		return;
	}

	if (IsValid(MeleeAttackMontage))
	{
		PlayAnimMontage(MeleeAttackMontage);
	}

	// �������� �Ҹ� ���
	if (IsValid(CurrentGameState))
	{
		CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::MeleeAttack);
	}

	// ���� ���� ������
	GetWorld()->GetTimerManager().SetTimer(
		MeleeAttackDelayHandle,
		this,
		&APlayerCharacterFey::EndMeleeAttack,
		MeleeAttackDelay,
		false
	);

	IsMeleeAttack = true;
}

void APlayerCharacterFey::EndMeleeAttack()
{
	IsMeleeAttack = false;
}

void APlayerCharacterFey::Attack(const FInputActionValue& value)
{
	if (Stamina < 10 || IsAttack)
	{
		return;
	}

	Super::Attack(value);

	IsAttack = true;
	Stamina -= 10;

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerOther(MaxStamina, Stamina);
	}

	if (IsValid(AttackMontage))
	{
		PlayAnimMontage(AttackMontage);
	}
}

void APlayerCharacterFey::ActiveWeapon()
{
	if (IsValid(WeaponActor))
	{
		if (WeaponActor->Fire(AttackDelay))
		{
			GetWorld()->GetTimerManager().SetTimer(
				AttackDelayHandle,
				this,
				&APlayerCharacterFey::SetIsAttack,
				AttackDelay,
				false
			);
		}
	}
}

void APlayerCharacterFey::AttackTrace()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FRotator ControlRotation = PlayerController->GetControlRotation();
	FVector ForwardVector = ControlRotation.Vector();

	FVector Start = GetActorLocation() + (ForwardVector * (MeleeAttackRadius + 50)); // ���� ���� ��ġ
	FVector End = Start + (ForwardVector * (MeleeAttackRadius + 50)); // ���� �� ��ġ

	// ���� ���� ������ �浹 üũ
	float Radius = MeleeAttackRadius;
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
		FCollisionShape::MakeSphere(Radius), // ���� ���� (��ü ���)
		QueryParams
	);

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

			if (HitActor)
			{
				continue;
			}

			if (HitActor->ActorHasTag("Boss"))
			{
				continue;
			}

			if (!DamagedActors.Contains(HitActor) && (HitActor->ActorHasTag("MissionItem") || HitActor->ActorHasTag("Monster")))
			{
				// �̼� ������
				if (HitActor->ActorHasTag("MissionItem"))
				{
					UGameplayStatics::ApplyDamage(
						HitActor,
						GetAttackDamage(),
						nullptr,
						this,
						UDamageType::StaticClass()
					);
				}

				// ���ʹ� ��ġ��
				else if (HitActor->ActorHasTag("Monster"))
				{
					UPrimitiveComponent* HitPrimitive = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());

					if (HitPrimitive)
					{
						ACharacter* HitCharacter = Cast<ACharacter>(HitActor);

						if (HitCharacter)
						{
							HitCharacter->GetCharacterMovement()->StopMovementImmediately();
							FVector LaunchVector = GetActorForwardVector() * MeleeAttackPushStrength;
							LaunchVector.Z = 300;
							HitCharacter->LaunchCharacter(LaunchVector, false, false);
						}
					}
				}
			}

			// �������� ���� ���͸� Set�� �߰�
			DamagedActors.Add(HitActor);
		}
	}
}

void APlayerCharacterFey::SetIsHover()
{
	IsHover = false;
}

void APlayerCharacterFey::SetIsAttack()
{
	IsAttack = false;
}

bool APlayerCharacterFey::CheckAction()
{
	return IsDie || IsAttack || IsMeleeAttack;
}
