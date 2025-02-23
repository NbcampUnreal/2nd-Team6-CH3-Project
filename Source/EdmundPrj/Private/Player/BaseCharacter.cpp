#include "Player/BaseCharacter.h"
#include "Player/EdmundPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

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

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	IsSprint = false;
	IsCrouch = false;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

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
	if (!Controller) return;

	FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ABaseCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ABaseCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ABaseCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ABaseCharacter::StartSprint(const FInputActionValue& value)
{
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
	if (CurrentAmmo <= 0)
	{
		return;
	}

	if (ActiveWeapon())
	{
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
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("MeleeAttack Start!!")));
}

void ABaseCharacter::ReloadAction(const FInputActionValue& value)
{
	Reload();
}

void ABaseCharacter::Reload()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Reload Start!!")));
	CurrentAmmo = MaxAmmo;
}

void ABaseCharacter::Interaction(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Interaction Start!!")));
}

void ABaseCharacter::ZoomIn(const FInputActionValue& value)
{
	if (!IsValid(SpringArmComp))
	{
		return;
	}

	SpringArmComp->TargetArmLength = -1000;
	//SpringArmComp->SocketOffset = FVector(0, 40, 60);
}

void ABaseCharacter::ZoomOut(const FInputActionValue& value)
{
	if (!IsValid(SpringArmComp))
	{
		return;
	}

	SpringArmComp->TargetArmLength = 300;
	//SpringArmComp->SocketOffset = FVector(0, 60, 60);
}

void ABaseCharacter::StartCrouch(const FInputActionValue& value)
{
	if (!IsCrouch && GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchMoveSpeed;

		// ĸ�� �ݸ��� ��������
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();

		if (IsValid(CapsuleComp))
		{
			// ĸ�� ũ�� ����
			CapsuleComp->SetCapsuleHalfHeight(CapsuleHeight / 2);

			// �޽� ��ġ ����
			FVector NewLocation = GetMesh()->GetRelativeLocation();
			NewLocation.Z += CapsuleHeight / 2;
			GetMesh()->SetRelativeLocation(NewLocation);

			// ī�޶� ��ġ ����
			NewLocation = SpringArmComp->GetRelativeLocation();
			NewLocation.Z += CapsuleHeight / 2;
			SpringArmComp->SetRelativeLocation(NewLocation);

			// ��ü ���� ����
			NewLocation = GetActorLocation();
			NewLocation.Z -= CapsuleHeight / 2;
			SetActorLocation(NewLocation);
		}

		IsCrouch = true;
	}
}

void ABaseCharacter::StopCrouch(const FInputActionValue& value)
{
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
			NewLocation.Z -= CapsuleHeight / 2;
			GetMesh()->SetRelativeLocation(NewLocation);

			// ī�޶� ��ġ ����
			NewLocation = SpringArmComp->GetRelativeLocation();
			NewLocation.Z -= CapsuleHeight / 2;
			SpringArmComp->SetRelativeLocation(NewLocation);

			// ��ü ���� ����
			NewLocation = GetActorLocation();
			NewLocation.Z += CapsuleHeight / 2;
			SetActorLocation(NewLocation);
		}

		IsCrouch = false;
	}
}

void ABaseCharacter::TakeDamage(float Damage)
{
	HP = FMath::Max(0.0f, HP - Damage);
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

ECharacterType ABaseCharacter::getCharacterType()
{
	return CharacterType;
}
