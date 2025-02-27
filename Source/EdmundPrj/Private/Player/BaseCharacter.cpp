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
#include "System/EdmundGameState.h"

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

	CurrentAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	CurrentAudioComp->SetupAttachment(RootComponent);

	WalkSpeed = 600.0f;
	SprintSpeed = 1000.0f;
	CrouchMoveSpeed = 300.0f;

	HP = MaxHP = 300.0f;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	IsSprint = false;
	IsCrouch = false;
	IsDie = false;

	HitActionMontage = nullptr;
	DieActionMontage = nullptr;
	CurrentGameState = nullptr;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	HP = MaxHP;

	// 캡슐 콜리전 크기 저장하기
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();

	if (IsValid(CapsuleComp))
	{
		CapsuleHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
	}

	AGameStateBase* GameStateBase = GetWorld()->GetGameState();

	CurrentGameState = Cast<AEdmundGameState>(GameStateBase);

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerHp(MaxHP, HP);
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

			if (PlayerController->InteractionAction)
			{
				EnhancedInput->BindAction(
					PlayerController->InteractionAction,
					ETriggerEvent::Started,
					this,
					&ABaseCharacter::Interaction
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

		// 앉은 상태에서는 달리기 불가
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

		// 앉은 상태에서는 달리기 불가
		if (!IsCrouch)
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
}

void ABaseCharacter::Interaction(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Interaction Start!!")));
	CurrentGameState->RequestInteraction();
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

		// 캡슐 콜리전 가져오기
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();

		if (IsValid(CapsuleComp))
		{
			float NewCapsuleHeight = CapsuleHeight * 0.7;

			// 캡슐 크기 조정
			CapsuleComp->SetCapsuleHalfHeight(CapsuleHeight * 0.8);

			// 메쉬 위치 조정
			FVector NewLocation = GetMesh()->GetRelativeLocation();
			NewLocation.Z += CapsuleHeight * 0.2;
			GetMesh()->SetRelativeLocation(NewLocation);

			// 카메라 위치 조정
			NewLocation = SpringArmComp->GetRelativeLocation();
			NewLocation.Z += CapsuleHeight * 0.2;
			SpringArmComp->SetRelativeLocation(NewLocation);

			// 전체 액터 조정
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

		// 캡슐 콜리전 가져오기
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();

		if (IsValid(CapsuleComp))
		{
			// 캡슐 크기 조정
			CapsuleComp->SetCapsuleHalfHeight(CapsuleHeight);

			// 메쉬 위치 조정
			FVector NewLocation = GetMesh()->GetRelativeLocation();
			NewLocation.Z -= CapsuleHeight * 0.2;
			GetMesh()->SetRelativeLocation(NewLocation);

			// 카메라 위치 조정
			NewLocation = SpringArmComp->GetRelativeLocation();
			NewLocation.Z -= CapsuleHeight * 0.2;
			SpringArmComp->SetRelativeLocation(NewLocation);

			// 전체 액터 조정
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

	if (IsValid(HitActionMontage) && !CheckAction())
	{
		PlayAnimMontage(HitActionMontage);
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// HP 음수 방지
	HP = FMath::Max(0.0f, HP - ActualDamage);

	if (HP == 0 && !IsDie)
	{
		IsDie = true;
		ActiveDieAction();
	}

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerHp(MaxHP, HP);
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

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerHp(MaxHP, HP);
	}
}

int32 ABaseCharacter::GetHP() const
{
	return HP;
}

void ABaseCharacter::SetHP(int32 NewHP)
{
	HP = FMath::Min(MaxHP, NewHP);

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerHp(MaxHP, HP);
	}
}

void ABaseCharacter::AmountHP(int32 AmountHP)
{
	HP = FMath::Min(MaxHP, HP + AmountHP);

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerHp(MaxHP, HP);
	}
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

ECharacterType ABaseCharacter::GetCharacterType()
{
	return CharacterType;
}

void ABaseCharacter::AttackTrace()
{
	// 자식 클래스 함수 실행
}

bool ABaseCharacter::CheckAction()
{
	return IsDie;
}
