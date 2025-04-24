#include "Player/BaseCharacter.h"
#include "Player/EdmundPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
//#include "DrawDebugHelpers.h"
//#include "Kismet/GameplayStatics.h"
//#include "Sound/SoundBase.h"
#include "System/EdmundGameState.h"
#include "System/DataStructure/ShopCatalogRow.h"
#include "Player/SkillManager.h"
#include "Player/TimerSkillSpawnManagerComponent.h"
#include "Player/ActiveSkillSpawnManager.h"
#include "Player\PassiveSkillManager.h"
#include "Player/ElectricEffectPool.h"
#include "System/EnumSet.h"
#include "Player\SupportCharacter.h"

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

	PassiveSkillManager = CreateDefaultSubobject<UPassiveSkillManager>(TEXT("PassiveSkillManager"));

	TimerSkillSpawnManagerComponent = CreateDefaultSubobject<UTimerSkillSpawnManagerComponent>(TEXT("TimerSkillManager"));
	TimerSkillSpawnManagerComponent->SetupAttachment(RootComponent);

	ActiveSkillSpawnManager = CreateDefaultSubobject<UActiveSkillSpawnManager>(TEXT("ActiveSkillManager"));
	ActiveSkillSpawnManager->SetupAttachment(RootComponent);

	ElectricEffectPool = CreateDefaultSubobject<UElectricEffectPool>(TEXT("ElectricEffectPool"));

	WalkSpeed = 600.0f;
	SprintSpeed = 1000.0f;
	CrouchMoveSpeed = 300.0f;

	HP = MaxHP = 200;
	Stamina = MaxStamina = 100;
	StaminaRecoveryAmount = 1.0f;
	StaminaConsumAmount = 4.0f;

	StaminaRecoveryAndConsumDelay = 1.0f;

	AttackDamage = 30;
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
	IsAttack = false;

	HitActionMontage = nullptr;
	DieActionMontage = nullptr;
	CurrentGameState = nullptr;

	CanCrouchCharacter = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

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
		CurrentGameState->NotifyPlayerOther(MaxStamina, Stamina);
		CurrentGameState->NotifyPlayerHp(MaxHP, HP);
		CurrentGameState->NotifyPlayerExp(MaxExp, CurrentExp);
		CurrentGameState->NotifyPlayerLevel(CurrentLevel);
	}

	GetWorld()->GetTimerManager().SetTimer(
		StaminaDelayHandle,
		this,
		&ABaseCharacter::UpdateStamina,
		StaminaRecoveryAndConsumDelay,
		true
	);

	if (IsValid(SupportCharClass))
	{
		SupportCharInstance = GetWorld()->SpawnActor<ASupportCharacter>(SupportCharClass, GetActorLocation(), GetActorRotation());
		if (IsValid(SupportCharInstance))
		{
			SupportCharInstance->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	
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
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Started, this, &ABaseCharacter::StartJump);
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopJump);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &ABaseCharacter::StartSprint);
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopSprint);
			}

			if (PlayerController->AttackAction)
			{
				EnhancedInput->BindAction(PlayerController->AttackAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Attack);
			}

			if (PlayerController->InteractionAction)
			{
				EnhancedInput->BindAction(PlayerController->InteractionAction, ETriggerEvent::Started, this, &ABaseCharacter::Interaction);
			}

			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(PlayerController->CrouchAction, ETriggerEvent::Triggered, this, &ABaseCharacter::StartCrouch);
				EnhancedInput->BindAction(PlayerController->CrouchAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopCrouch);
			}

			if (PlayerController->PauseAction)
			{
				EnhancedInput->BindAction(PlayerController->PauseAction, ETriggerEvent::Started, this, &ABaseCharacter::PauseAction);
			}

			if (PlayerController->MissionOnAction)
			{
				EnhancedInput->BindAction(PlayerController->MissionOnAction, ETriggerEvent::Started, this, &ABaseCharacter::MissionOnAction);
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
	
	AddControllerPitchInput(LookInput.Y);
	AddControllerYawInput(LookInput.X);
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

		if (!IsCrouch)
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
}

void ABaseCharacter::Attack(const FInputActionValue& value)
{
	ActiveSkillSpawnManager->ActivateProbCalculate();
}

void ABaseCharacter::Interaction(const FInputActionValue& value)
{
	CurrentGameState->RequestInteraction();
}

void ABaseCharacter::StartCrouch(const FInputActionValue& value)
{
	if (IsDie || !CanCrouchCharacter)
	{
		return;
	}

	if (!IsCrouch && GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchMoveSpeed;

		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();

		if (IsValid(CapsuleComp))
		{
			float NewCapsuleHeight = CapsuleHeight * 0.7;

			CapsuleComp->SetCapsuleHalfHeight(CapsuleHeight * 0.8);

			FVector NewLocation = GetMesh()->GetRelativeLocation();
			NewLocation.Z += CapsuleHeight * 0.2;
			GetMesh()->SetRelativeLocation(NewLocation);

			NewLocation = SpringArmComp->GetRelativeLocation();
			NewLocation.Z += CapsuleHeight * 0.2;
			SpringArmComp->SetRelativeLocation(NewLocation);

			NewLocation = GetActorLocation();
			NewLocation.Z -= CapsuleHeight * 0.2;
			SetActorLocation(NewLocation);
		}

		IsCrouch = true;
	}
}

void ABaseCharacter::StopCrouch(const FInputActionValue& value)
{
	if (IsDie || !CanCrouchCharacter)
	{
		return;
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();

		if (IsValid(CapsuleComp))
		{
			CapsuleComp->SetCapsuleHalfHeight(CapsuleHeight);

			FVector NewLocation = GetMesh()->GetRelativeLocation();
			NewLocation.Z -= CapsuleHeight * 0.2;
			GetMesh()->SetRelativeLocation(NewLocation);

			NewLocation = SpringArmComp->GetRelativeLocation();
			NewLocation.Z -= CapsuleHeight * 0.2;
			SpringArmComp->SetRelativeLocation(NewLocation);

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

void ABaseCharacter::MissionOnAction(const FInputActionValue& value)
{
	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyOnMissionInfo();
	}
}

float ABaseCharacter::GetAttackDamage() const
{
	float Damage = AttackDamage;

	int32 DamageProb = FMath::RandRange(1, 100);

	if (DamageProb <= CriticalProb)
	{
		Damage *= CriticalMultiplier;
	}

	float RandomRange = 0.2f;

	float MinDamage = (1.0f - RandomRange) * Damage;
	float MaxDamage = (1.0f + RandomRange) * Damage;

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

float ABaseCharacter::GetHeight() const
{
	return Height;
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDie)
	{
		return 0.0f;
	}

	int32 DamageProb = FMath::RandRange(1, 100);

	if (DamageProb <= EvasionProb)
	{
		if (IsValid(CurrentGameState))
		{
			CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Avoid);
		}

		return 0.0f;
	}

	if (IsValid(HitActionMontage) && !CheckAction() && !IsAttack)
	{
		PlayAnimMontage(HitActionMontage);
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ActualDamage = (100 - Defense) * ActualDamage / 100;
	
	HP = FMath::Max(0.0f, HP - ActualDamage);

	if (HP == 0 && !IsDie)
	{
		if (RevivalCount >= 1)
		{
			RevivalCount--;
			HP = MaxHP;

			if (IsValid(CurrentGameState))
			{
				CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Respawn);
			}
		}
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
	OnBerserkerSkillActivate.Broadcast();

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

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerExp(MaxExp, CurrentExp);
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
		CurrentGameState->NotifyPlayerHp(MaxHP, HP);
		CurrentGameState->NotifyPlayerOther(MaxStamina, Stamina);
		CurrentGameState->NotifyPlayerLevel(CurrentLevel);
		CurrentGameState->CreateRandomSkillSet();
	}
}

int32 ABaseCharacter::GetMaxHP() const
{
	return MaxHP;
}

void ABaseCharacter::SetMaxHP(const int32 NewMaxHP)
{
	MaxHP = NewMaxHP;
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

	OnBerserkerSkillActivate.Broadcast();
}

void ABaseCharacter::AmountHP(int32 AmountHP)
{
	HP = FMath::Min(MaxHP, HP + AmountHP);

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerHp(MaxHP, HP);
	}
	OnBerserkerSkillActivate.Broadcast();
}

int32 ABaseCharacter::GetMaxStamina() const
{
	return MaxStamina;
}

void ABaseCharacter::SetMaxStamina(const int32 NewMaxStamina)
{
	MaxStamina = NewMaxStamina;
}

int32 ABaseCharacter::GetCurrentStamina() const
{
	return Stamina;
}

int32 ABaseCharacter::GetStaminaRecoveryAmount() const
{
	return StaminaRecoveryAmount;
}

void ABaseCharacter::SetStaminaRecoveryAmount(int32 NewStaminaRecoveryAmount)
{
	StaminaRecoveryAmount = NewStaminaRecoveryAmount;
}

float ABaseCharacter::GetExpMultipler() const
{
	return ExpMultipler;
}

float ABaseCharacter::GetGoldMultipler() const
{
	return GoldMultipler;
}

int32 ABaseCharacter::GetItempDropProb() const
{
	return ItemDropProb;
}

void ABaseCharacter::GetUpgradeStatus()
{
	check(CurrentGameState);

	TArray<FShopCatalogRow*> ShopStatusList = CurrentGameState->GetPlayerAdvancedData();

	// MaxHP
	MaxHP = MaxHP + ShopStatusList[0]->CurrentLevel * ShopStatusList[0]->AdvanceValue;

	// Damage
	AttackDamage = AttackDamage + ShopStatusList[1]->CurrentLevel * ShopStatusList[1]->AdvanceValue;

	// CriticalRate
	CriticalProb = CriticalProb + ShopStatusList[2]->CurrentLevel * ShopStatusList[2]->AdvanceValue;

	// AttackSpeed
	AttackDelay = AttackDelay * (1.0f - ShopStatusList[3]->CurrentLevel * ShopStatusList[3]->AdvanceValue * 0.1);

	// MoveSpeed
	float SpeedMultipler = 1.0f + ShopStatusList[4]->CurrentLevel * ShopStatusList[4]->AdvanceValue;

	WalkSpeed *= SpeedMultipler;
	SprintSpeed *= SpeedMultipler;
	CrouchMoveSpeed *= SpeedMultipler;

	// AvoidRate
	EvasionProb = EvasionProb + ShopStatusList[5]->CurrentLevel * ShopStatusList[5]->AdvanceValue;

	// Defence
	Defense = Defense + ShopStatusList[6]->CurrentLevel * ShopStatusList[6]->AdvanceValue;

	// ExpAmount
	ExpMultipler = ExpMultipler + ShopStatusList[7]->CurrentLevel * ShopStatusList[7]->AdvanceValue;

	// GoldAmount
	GoldMultipler = GoldMultipler + ShopStatusList[8]->CurrentLevel * ShopStatusList[8]->AdvanceValue;

	// DropRate
	ItemDropProb = ItemDropProb + ShopStatusList[9]->CurrentLevel * ShopStatusList[9]->AdvanceValue;

	// MaxAmmo
	MaxStamina = MaxStamina * (100.0f + ShopStatusList[10]->CurrentLevel * ShopStatusList[10]->AdvanceValue) / 100;

	// ReloadTime
	RevivalCount = ShopStatusList[12]->CurrentLevel * ShopStatusList[12]->AdvanceValue;
}

void ABaseCharacter::ActiveDieAction()
{
	if (IsValid(CurrentGameState))
	{
		CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Die);
	}

	if (IsValid(DieActionMontage))
	{
		PlayAnimMontage(DieActionMontage);
	}
}

ECharacterType ABaseCharacter::GetCharacterType() const
{
	return CharacterType;
}

void ABaseCharacter::AttackTrace()
{
	// Call the overridden function in the derived class
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

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerOther(MaxStamina, Stamina);
	}
}

bool ABaseCharacter::CheckAction()
{
	return IsDie;
}
