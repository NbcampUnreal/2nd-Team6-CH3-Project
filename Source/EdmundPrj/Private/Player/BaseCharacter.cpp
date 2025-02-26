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

	CurrentAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	CurrentAudioComp->SetupAttachment(RootComponent);

	WalkSpeed = 600.0f;
	SprintSpeed = 1000.0f;
	CrouchMoveSpeed = 300.0f;

	CurrentAmmo = MaxAmmo = 40;

	HP = MaxHP = 300.0f;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	IsSprint = false;
	IsCrouch = false;

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

	// 캡슐 콜리전 크기 저장하기
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
}

void ABaseCharacter::Look(const FInputActionValue& value)
{
	if (IsDie)
	{
		return;
	}

	FVector2D LookInput = value.Get<FVector2D>();

	// 줌상태에서 마우스 감도 낮추기
	if (IsZoom)
	{
		LookInput /= 4;
	}

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
	// 자식 클래스의 함수 실행
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

	// 근접공격 소리 재생
	if (MeleeAttackSound)
	{
		//UGameplayStatics::PlaySoundAtLocation(this, MeleeAttackSound, GetActorLocation());
		CurrentAudioComp->SetSound(MeleeAttackSound);
		CurrentAudioComp->Play();
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Melee Attack Start")));

	// 근접 공격 딜레이
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
	// Melee Attack 범위 설정
	//FVector ForwardVector = GetActorForwardVector(); // 공격 방향

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FRotator ControlRotation = PlayerController->GetControlRotation();

	FVector ForwardVector = ControlRotation.Vector();

	FVector Start = GetActorLocation() + (ForwardVector * 200.0f); // 공격 시작 위치
	FVector End = Start + (ForwardVector * 200.0f); // 공격 끝 위치

	// 공격 범위 내에서 충돌 체크
	float Radius = 150.0f;
	TArray<FHitResult> HitResults;

	// 트레이스 수행
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 자신은 무시하도록 설정

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,               // 시작 위치
		End,                 // 끝 위치
		FQuat::Identity,     // 회전값 (회전 없이)
		ECollisionChannel::ECC_OverlapAll_Deprecated, // 충돌 채널
		FCollisionShape::MakeSphere(Radius), // 범위 설정 (구체 모양)
		QueryParams
	);

	// 구체 보이게 하기
	if (GEngine)
	{
		DrawDebugSphere(
			GetWorld(),
			Start,
			Radius,
			12,
			FColor::Red,        // 색상
			false,              // 지속성 (게임 중 계속 표시할지 여부)
			1.0f                // 지속 시간
		);
	}

	// 데미지를 입힌 액터를 추적할 Set (중복 방지)
	// Set이 없으면 근접공격한번에 여러번 데미지 받는 현상 발생
	TSet<AActor*> DamagedActors;

	if (bHit)
	{
		// 여러 충돌 객체가 있다면
		for (const FHitResult& Hit : HitResults)
		{
			// 충돌한 객체가 있다면
			AActor* HitActor = Hit.GetActor();

			if (!DamagedActors.Contains(HitActor) && HitActor && HitActor->ActorHasTag("Monster"))
			{
				UGameplayStatics::ApplyDamage(
					HitActor,
					30.0f,	// 수정필요
					nullptr,
					this,
					UDamageType::StaticClass()
				);
			}

			// 데미지를 입힌 액터를 Set에 추가
			DamagedActors.Add(HitActor);
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
		//UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
		CurrentAudioComp->SetSound(ReloadSound);
		CurrentAudioComp->Play();
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

	IsZoom = true;
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

	IsZoom = false;
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
	// 만약 줌상태라면
	IsZoom = false;
	SpringArmComp->TargetArmLength = 300;

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
