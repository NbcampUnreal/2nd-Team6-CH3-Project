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
	// 첫 번째 점프가 가능하면
	if (CanJump())
	{
		Super::Jump(); // 첫 번째 점프 수행
		JumpCount = 1; // 점프 후 2단 점프 가능
	}

	// 첫 번째 점프 후, 떨어지는 상태에서 두 번째 점프가 가능
	else if (JumpCount < 2) // 이미 공중에 있을 때 (IsFalling()이 true)
	{
		// 2단 점프 수행
		FVector JumpVelocity = GetVelocity();

		JumpVelocity.Normalize();
		JumpVelocity *= GetCharacterMovement()->MaxWalkSpeed;
		JumpVelocity.Z = DoubleJumpHeight;

		GetCharacterMovement()->Launch(JumpVelocity);  // Z축 방향으로 점프

		// 2단 점프 후, 2단 점프 불가능하게 설정
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
		CurrentVelocity.Z = HoveringZSpeed;		// 하강 속도 제한
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

	// 근접공격 소리 재생
	CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::MeleeAttack);

	// 근접 공격 딜레이
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

	FVector Start = GetActorLocation() + (ForwardVector * (MeleeAttackRadius + MeleeAttackForwardOffset)); // 공격 시작 위치
	FVector End = Start + (ForwardVector * (MeleeAttackRadius + MeleeAttackForwardOffset)); // 공격 끝 위치

	// 공격 범위 내에서 충돌 체크
	float Radius = MeleeAttackRadius;
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

	if (!bHit)
	{
		return;
	}

	// 여러 충돌 객체가 있다면
	for (const FHitResult& Hit : HitResults)
	{
		// 충돌한 객체가 있다면
		AActor* HitActor = Hit.GetActor();

		if (!IsValid(HitActor) || HitActor->ActorHasTag("Boss"))
		{
			continue;
		}

		// 미션 아이템 데미지 주기
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

		// 몬스터는 밀치기
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
