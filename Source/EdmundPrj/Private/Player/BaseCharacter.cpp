#include "Player/BaseCharacter.h"
#include "Player/EdmundPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	IsSprint = false;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		IsSprint = true;
	}
}

void ABaseCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		IsSprint = false;
	}
}

void ABaseCharacter::Reload()
{
	CurrentAmmo = MaxAmmo;
}

void ABaseCharacter::Interaction()
{
}

void ABaseCharacter::Attack()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
	}
}

void ABaseCharacter::MeleeAttack()
{
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
