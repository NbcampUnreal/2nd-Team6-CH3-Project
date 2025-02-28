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
#include "System/DataStructure/ShopCatalogRow.h"
#include "Player/SkillManager.h"
#include "Player/TimerSkillSpawnManagerComponent.h"
#include "Player/ActiveSkillSpawnManager.h"

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

	SkillManager = CreateDefaultSubobject<USkillManager>(TEXT("SkillManager"));

	TimerSkillSpawnManagerComponent = CreateDefaultSubobject<UTimerSkillSpawnManagerComponent>(TEXT("TimerSkillManager"));
	TimerSkillSpawnManagerComponent->SetupAttachment(RootComponent);

	ActiveSkillSpawnManager = CreateDefaultSubobject<UActiveSkillSpawnManager>(TEXT("ActiveSkillManager"));
	ActiveSkillSpawnManager->SetupAttachment(RootComponent);

	WalkSpeed = 600.0f;
	SprintSpeed = 1000.0f;
	CrouchMoveSpeed = 300.0f;

	HP = MaxHP = 300;
	Stamina = MaxStamina = 100;
	StaminaRecoveryAmount = StaminaConsumAmount = 0.0f;

	StaminaRecoveryAndConsumDelay = 1.0f;

	AttackDamage = 20;
	Defense = 10;
	AttackDelay = 0.5;
	CriticalProb = 5;
	CriticalMultiplier = 2.0f;
	EvasionProb = 5;
	MaxExp = 100;
	CurrentLevel = 1;
	MaxLevel = 30;

	ExpMultipler = 100.0f;
	GoldMultipler = 100.0f;
	ItemDropProb = 20;
	RevivalCount = 0;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	IsSprint = false;
	IsCrouch = false;
	IsDie = false;

	HitActionMontage = nullptr;
	DieActionMontage = nullptr;
	CurrentGameState = nullptr;

	EvasionSuccessSound = nullptr;
	RevivalSuccessSound = nullptr;
	DeathSound = nullptr;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// 캡슐 콜리전 크기 저장하기
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();

	if (IsValid(CapsuleComp))
	{
		CapsuleHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
	}

	AGameStateBase* GameStateBase = GetWorld()->GetGameState();

	CurrentGameState = Cast<AEdmundGameState>(GameStateBase);

	GetUpgradeStatus();

	HP = MaxHP;
	Stamina = MaxStamina;

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerHp(MaxHP, HP);
	}

	GetWorld()->GetTimerManager().SetTimer(
		StaminaDelayHandle,
		this,
		&ABaseCharacter::UpdateStamina,
		StaminaRecoveryAndConsumDelay,
		true
	);
}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(StaminaDelayHandle);
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

			if (PlayerController->InteractionAction)
			{
				EnhancedInput->BindAction(
					PlayerController->PauseAction,
					ETriggerEvent::Started,
					this,
					&ABaseCharacter::PauseAction
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

	if (Stamina < 4)
	{
		StopSprint(value);
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

void ABaseCharacter::PauseAction(const FInputActionValue& value)
{
	if (IsValid(CurrentGameState))
	{
		CurrentGameState->OnPressedPauseKey();
	}
}

float ABaseCharacter::GetAttackDamage() const
{
	float Damage = AttackDamage;

	int32 DamageProb = FMath::RandRange(1, 100);

	// 크리티컬
	if (DamageProb <= CriticalProb)
	{
		Damage *= CriticalMultiplier;
	}

	// 랜덤 데미지 적용
	int32 RandomRange = 20;

	float MinDamage = (100 - RandomRange) * Damage;
	float MaxDamage = (100 + RandomRange) * Damage;

	Damage = FMath::RandRange(MinDamage, MaxDamage);

	return Damage;
}

void ABaseCharacter::SetAttackDamage(float NewAttackDamage)
{
	AttackDamage = NewAttackDamage;
}

float ABaseCharacter::GetAttackDelay() const
{
	return AttackDelay;
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

	if (IsValid(HitActionMontage) && !CheckAction())
	{
		PlayAnimMontage(HitActionMontage);
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

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

void ABaseCharacter::AddExp(int32 Exp)
{
	if (CurrentLevel >= MaxLevel)
	{
		return;
	}

	CurrentExp += Exp;

	if (CurrentExp >= MaxExp)
	{
		LevelUp();
	}
}

void ABaseCharacter::LevelUp()
{
	if (CurrentLevel >= MaxLevel)
	{
		return;
	}

	CurrentLevel++;

	HP = MaxHP;

	CurrentExp -= MaxExp;
	MaxExp += 50;

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->CreateRandomSkillSet();
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
	HP = FMath::Max(0, HP);

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
	if (!IsValid(CurrentGameState))
	{
		return;
	}

	TArray<FShopCatalogRow*> ShopStatusList = CurrentGameState->GetPlayerAdvancedData();

	// MaxHP
	MaxHP = MaxHP + ShopStatusList[0]->CurrentLevel * ShopStatusList[0]->AdvanceValue;

	// Damage
	AttackDamage = AttackDamage + ShopStatusList[1]->CurrentLevel * ShopStatusList[1]->AdvanceValue;

	// CriticalRate
	CriticalProb = CriticalProb + ShopStatusList[2]->CurrentLevel * ShopStatusList[2]->AdvanceValue;

	// AttackSpeed
	// 자식 클래스에서

	// MoveSpeed
	float SpeedMultipler = 1.0f + ShopStatusList[4]->CurrentLevel * ShopStatusList[4]->AdvanceValue;

	WalkSpeed *= SpeedMultipler;
	SprintSpeed *= SpeedMultipler;
	CrouchMoveSpeed *= SpeedMultipler;

	// AvoidRate
	EvasionProb = EvasionProb + ShopStatusList[5]->CurrentLevel * ShopStatusList[5]->AdvanceValue;

	// Defence
	Defense = Defense * (1.0f + ShopStatusList[6]->CurrentLevel * ShopStatusList[6]->AdvanceValue);

	// ExpAmount
	ExpMultipler = ExpMultipler + ShopStatusList[7]->CurrentLevel * ShopStatusList[7]->AdvanceValue;

	// GoldAmount
	GoldMultipler = GoldMultipler + ShopStatusList[8]->CurrentLevel * ShopStatusList[8]->AdvanceValue;

	// DropRate
	ItemDropProb = ItemDropProb + ShopStatusList[9]->CurrentLevel * ShopStatusList[9]->AdvanceValue;

	// MaxAmmo
	// 자식 클래스에서

	// ReloadTime
	// 자식 클래스에서

	RevivalCount = ShopStatusList[12]->CurrentLevel * ShopStatusList[0]->AdvanceValue;
}

void ABaseCharacter::ActiveDieAction()
{
	// 회피 성공 사운드
	if (IsValid(DeathSound))
	{
		CurrentAudioComp->SetSound(DeathSound);
		CurrentAudioComp->Play();
	}

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

void ABaseCharacter::UpdateStamina()
{
	if (IsSprint && !IsCrouch)
	{
		Stamina -= StaminaConsumAmount;
	}

	else
	{
		Stamina += StaminaRecoveryAmount;
	}

	Stamina = FMath::Clamp(Stamina, 0, MaxStamina);

	CurrentGameState->NotifyPlayerOther(MaxStamina, Stamina);
}

bool ABaseCharacter::CheckAction()
{
	return IsDie;
}
