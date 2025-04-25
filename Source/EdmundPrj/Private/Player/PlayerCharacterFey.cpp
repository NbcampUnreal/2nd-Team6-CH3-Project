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
	MeleeAttackForwardOffset = 50.0f;

	AttackCost = 10;

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
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ThisClass::TriggerJump);
			}

			if (PlayerController->MeleeAttackAction)
			{
				EnhancedInput->BindAction(PlayerController->MeleeAttackAction, ETriggerEvent::Triggered, this, &ThisClass::MeleeAttack);
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

		JumpVelocity.Normalize();
		JumpVelocity *= GetCharacterMovement()->MaxWalkSpeed;
		JumpVelocity.Z = DoubleJumpHeight;

		GetCharacterMovement()->Launch(JumpVelocity);  // Z�� �������� ����

		// 2�� ���� ��, 2�� ���� �Ұ����ϰ� ����
		JumpCount++;

		IsHover = true;

		GetWorld()->GetTimerManager().SetTimer(
			HoveringDelayHandle,
			this,
			&ThisClass::SetIsHover,
			HoveringDelay,
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
	
	check(CurrentGameState);

	// �������� �Ҹ� ���
	CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::MeleeAttack);

	// ���� ���� ������
	GetWorld()->GetTimerManager().SetTimer(
		MeleeAttackDelayHandle,
		this,
		&ThisClass::EndMeleeAttack,
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
	if (Stamina < AttackCost || IsAttack)
	{
		return;
	}

	Super::Attack(value);

	IsAttack = true;
	Stamina -= AttackCost;

	check(CurrentGameState);

	CurrentGameState->NotifyPlayerOther(MaxStamina, Stamina);


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
				&ThisClass::SetIsAttack,
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

	FVector Start = GetActorLocation() + (ForwardVector * (MeleeAttackRadius + MeleeAttackForwardOffset)); // ���� ���� ��ġ
	FVector End = Start + (ForwardVector * (MeleeAttackRadius + MeleeAttackForwardOffset)); // ���� �� ��ġ

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

	if (!bHit)
	{
		return;
	}

	// ���� �浹 ��ü�� �ִٸ�
	for (const FHitResult& Hit : HitResults)
	{
		// �浹�� ��ü�� �ִٸ�
		AActor* HitActor = Hit.GetActor();

		if (!IsValid(HitActor) || HitActor->ActorHasTag("Boss"))
		{
			continue;
		}

		// �̼� ������ ������ �ֱ�
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
					LaunchVector.Z = MeleeAttackPushStrength * 0.3f;
					HitCharacter->LaunchCharacter(LaunchVector, false, false);
				}
			}
		}
	}
}

void APlayerCharacterFey::SetJumpCount()
{
	JumpCount = 1;
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
