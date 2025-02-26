#include "Player/BaseCharacter.h"
#include "Player/EdmundPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	WalkSpeed = 600.0f;
	SprintSpeed = 1000.0f;
	CrouchMoveSpeed = 300.0f;

	CurrentAmmo = MaxAmmo = 40;

	HP = MaxHP = 300.0f;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	IsSprint = false;
	IsCrouch = false;
	IsMove = false;

	MeleeAttackDelay = 0.7f;

	AttackMontage = nullptr;
	MeleeAttackMontage = nullptr;

	bIsMeleeAttack = false;
	bIsAttack = false;
	bIsReloading = false;

	HitActionMontage = nullptr;

	IsDie = false;
	DieActionMontage = nullptr;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	HP = MaxHP;

	// ĸ�� �ݸ��� ũ�� �����ϱ�
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();

	if (IsValid(CapsuleComp))
	{
		CapsuleHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AEdmundPlayerController* PlayerController = Cast<AEdmundPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ABaseCharacter::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ABaseCharacter::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Completed,
					this,
					&ABaseCharacter::StopJump
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ABaseCharacter::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ABaseCharacter::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ABaseCharacter::StopSprint
				);
			}

			if (PlayerController->AttackAction)
			{
				EnhancedInput->BindAction(
					PlayerController->AttackAction,
					ETriggerEvent::Triggered,
					this,
					&ABaseCharacter::Attack
				);
			}

			if (PlayerController->MeleeAttackAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MeleeAttackAction,
					ETriggerEvent::Triggered,
					this,
					&ABaseCharacter::MeleeAttack
				);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Triggered,
					this,
					&ABaseCharacter::ReloadAction
				);
			}

			if (PlayerController->InteractionAction)
			{
				EnhancedInput->BindAction(
					PlayerController->InteractionAction,
					ETriggerEvent::Triggered,
					this,
					&ABaseCharacter::Interaction
				);
			}

			if (PlayerController->ZoomAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ZoomAction,
					ETriggerEvent::Started,
					this,
					&ABaseCharacter::ZoomIn
				);

				EnhancedInput->BindAction(
					PlayerController->ZoomAction,
					ETriggerEvent::Completed,
					this,
					&ABaseCharacter::ZoomOut
				);
			}

			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Triggered,
					this,
					&ABaseCharacter::StartCrouch
				);

				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Completed,
					this,
					&ABaseCharacter::StopCrouch
				);
			}
		}
	}
}

void ABaseCharacter::Move(const FInputActionValue& value)
{
	if (!Controller || IsDie) return;

	FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}

	IsMove = true;
}

void ABaseCharacter::StopMove(const FInputActionValue& value)
{
	IsMove = false;
}

void ABaseCharacter::Look(const FInputActionValue& value)
{
	if (IsDie)
	{
		return;
	}

	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ABaseCharacter::StartJump(const FInputActionValue& value)
{
	if (IsDie)
	{
		return;
	}

	if (value.Get<bool>())
	{
		Jump();
	}
}

void ABaseCharacter::StopJump(const FInputActionValue& value)
{
	if (IsDie)
	{
		return;
	}

	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ABaseCharacter::StartSprint(const FInputActionValue& value)
{
	if (IsDie)
	{
		return;
	}

	if (GetCharacterMovement())
	{
		IsSprint = true;

		// ���� ���¿����� �޸��� �Ұ�
		if (!IsCrouch)
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
	}
}

void ABaseCharacter::StopSprint(const FInputActionValue& value)
{
	if (IsDie)
	{
		return;
	}

	if (GetCharacterMovement())
	{
		IsSprint = false;

		// ���� ���¿����� �޸��� �Ұ�
		if (!IsCrouch)
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
}

void ABaseCharacter::Attack(const FInputActionValue& value)
{
	if (CurrentAmmo <= 0 || CheckAction() || IsDie)
	{
		return;
	}

	if (ActiveWeapon())
	{
		if (IsValid(AttackMontage))
		{
			PlayAnimMontage(AttackMontage);
		}

		CurrentAmmo--;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Ammo: %d/%d"), CurrentAmmo, MaxAmmo));
	}

	if (CurrentAmmo <= 0)
	{
		Reload();
	}
}

bool ABaseCharacter::ActiveWeapon()
{
	// �ڽ� Ŭ������ �Լ� ����
	return false;
}

void ABaseCharacter::MeleeAttack(const FInputActionValue& value)
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
	if (MeleeAttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, MeleeAttackSound, GetActorLocation());
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Melee Attack Start")));

	// ���� ���� �����
	GetWorld()->GetTimerManager().SetTimer(
		MeleeAttackDelayHandle,
		this,
		&ABaseCharacter::EndMeleeAttack,
		MeleeAttackDelay,
		false
	);

	bIsMeleeAttack = true;
}

void ABaseCharacter::MeleeAttackTrace()
{
	// Melee Attack ���� ����
	FVector ForwardVector = GetActorForwardVector(); // ���� ����
	FVector Start = GetActorLocation() + (ForwardVector * 200.0f); // ���� ���� ��ġ
	FVector End = Start + (ForwardVector * 200.0f); // ���� �� ��ġ

	// ���� ���� ������ �浹 üũ
	float Radius = 150.0f;
	FHitResult HitResult;

	// Ʈ���̽� ����
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // �ڽ��� �����ϵ��� ����

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,               // ���� ��ġ
		End,                 // �� ��ġ
		FQuat::Identity,     // ȸ���� (ȸ�� ����)
		ECollisionChannel::ECC_GameTraceChannel1, // �浹 ä��
		FCollisionShape::MakeSphere(Radius), // ���� ���� (��ü ���)
		QueryParams
	);

	// ��ü ���̰� �ϱ�
	if (GEngine)
	{
		DrawDebugSphere(
			GetWorld(),
			Start,
			Radius,
			12,
			FColor::Red,        // ����
			false,              // ���Ӽ� (���� �� ��� ǥ������ ����)
			1.0f                // ���� �ð�
		);
	}

	if (bHit)
	{
		// �浹�� ��ü�� �ִٸ�
		if (AActor* HitActor = HitResult.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Melee attack hit: %s"), *HitActor->GetName());
		}
	}
}

void ABaseCharacter::EndMeleeAttack()
{
	bIsMeleeAttack = false;
}

void ABaseCharacter::ReloadAction(const FInputActionValue& value)
{
	if (CheckAction() || IsDie)
	{
		return;
	}

	Reload();
}

void ABaseCharacter::Reload()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Reload Start!!")));
	CurrentAmmo = MaxAmmo;

	if (IsValid(ReloadSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
	}
}

void ABaseCharacter::Interaction(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Interaction Start!!")));
}

void ABaseCharacter::ZoomIn(const FInputActionValue& value)
{
	if (IsDie)
	{
		return;
	}

	if (!IsValid(SpringArmComp))
	{
		return;
	}

	SpringArmComp->TargetArmLength = -1000;
	//SpringArmComp->SocketOffset = FVector(0, 40, 60);
}

void ABaseCharacter::ZoomOut(const FInputActionValue& value)
{
	if (IsDie)
	{
		return;
	}

	if (!IsValid(SpringArmComp))
	{
		return;
	}

	SpringArmComp->TargetArmLength = 300;
	//SpringArmComp->SocketOffset = FVector(0, 60, 60);
}

void ABaseCharacter::StartCrouch(const FInputActionValue& value)
{
	if (IsDie)
	{
		return;
	}

	if (!IsCrouch && GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchMoveSpeed;

		// ĸ�� �ݸ��� ��������
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();

		if (IsValid(CapsuleComp))
		{
			float NewCapsuleHeight = CapsuleHeight * 0.7;

			// ĸ�� ũ�� ����
			CapsuleComp->SetCapsuleHalfHeight(CapsuleHeight * 0.8);

			// �޽� ��ġ ����
			FVector NewLocation = GetMesh()->GetRelativeLocation();
			NewLocation.Z += CapsuleHeight * 0.2;
			GetMesh()->SetRelativeLocation(NewLocation);

			// ī�޶� ��ġ ����
			NewLocation = SpringArmComp->GetRelativeLocation();
			NewLocation.Z += CapsuleHeight * 0.2;
			SpringArmComp->SetRelativeLocation(NewLocation);

			// ��ü ���� ����
			NewLocation = GetActorLocation();
			NewLocation.Z -= CapsuleHeight * 0.2;
			SetActorLocation(NewLocation);
		}

		IsCrouch = true;
	}
}

void ABaseCharacter::StopCrouch(const FInputActionValue& value)
{
	if (IsDie)
	{
		return;
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

		// ĸ�� �ݸ��� ��������
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();

		if (IsValid(CapsuleComp))
		{
			// ĸ�� ũ�� ����
			CapsuleComp->SetCapsuleHalfHeight(CapsuleHeight);

			// �޽� ��ġ ����
			FVector NewLocation = GetMesh()->GetRelativeLocation();
			NewLocation.Z -= CapsuleHeight * 0.2;
			GetMesh()->SetRelativeLocation(NewLocation);

			// ī�޶� ��ġ ����
			NewLocation = SpringArmComp->GetRelativeLocation();
			NewLocation.Z -= CapsuleHeight * 0.2;
			SpringArmComp->SetRelativeLocation(NewLocation);

			// ��ü ���� ����
			NewLocation = GetActorLocation();
			NewLocation.Z += CapsuleHeight * 0.2;
			SetActorLocation(NewLocation);
		}

		IsCrouch = false;
	}
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDie)
	{
		return 0.0f;
	}

	if (IsValid(HitActionMontage))
	{
		PlayAnimMontage(HitActionMontage);
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// HP ���� ����
	HP = FMath::Max(0.0f, HP - ActualDamage);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("HP: %f / %f"), HP, MaxHP));

	if (HP == 0 && !IsDie)
	{
		IsDie = true;
		ActiveDieAction();
	}

	return ActualDamage;
}

void ABaseCharacter::AddExp(int32 Exp)
{
	CurrentExp += Exp;
}

void ABaseCharacter::LevelUp()
{
	CurrentLevel++;
}

float ABaseCharacter::GetHP() const
{
	return HP;
}

void ABaseCharacter::SetHP(float NewHP)
{
	HP = FMath::Min(MaxHP, NewHP);
}

void ABaseCharacter::AmountHP(float AmountHP)
{
	HP = FMath::Min(MaxHP, HP + AmountHP);
}

void ABaseCharacter::SetAmmo(int32 NewAmmo)
{
	CurrentAmmo = FMath::Min(MaxAmmo, NewAmmo);
}

void ABaseCharacter::AmountAmmo(int32 AmountAmmo)
{
	CurrentAmmo = FMath::Min(MaxAmmo, CurrentAmmo + AmountAmmo);
}

int32 ABaseCharacter::GetGold() const
{
	return CurrentGold;
}

void ABaseCharacter::AddGold(int32 Gold)
{
	CurrentGold += Gold;
}

void ABaseCharacter::GetUpgradeStatus()
{
}

void ABaseCharacter::ActiveDieAction()
{
	if (IsValid(DieActionMontage))
	{
		PlayAnimMontage(DieActionMontage);
	}
}

ECharacterType ABaseCharacter::getCharacterType()
{
	return CharacterType;
}

bool ABaseCharacter::CheckAction()
{
	return bIsAttack || bIsReloading || bIsMeleeAttack;
}
