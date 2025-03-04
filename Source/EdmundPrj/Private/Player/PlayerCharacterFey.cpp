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

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		bool IsMove = false;

		// 특정 키(W 키)가 눌렸는지 확인
		if (PlayerController && PlayerController->IsInputKeyDown(EKeys::W))
		{
			IsMove = true;
			JumpVelocity = GetActorForwardVector() * GetCharacterMovement()->MaxWalkSpeed;
		}

		// 특정 키(S 키)가 눌렸는지 확인
		else if (PlayerController && PlayerController->IsInputKeyDown(EKeys::S))
		{
			IsMove = true;
			JumpVelocity = GetActorForwardVector() * -1.0f * GetCharacterMovement()->MaxWalkSpeed;
		}

		// 특정 키(A 키)가 눌렸는지 확인
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

		// 특정 키(D 키)가 눌렸는지 확인
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

		GetCharacterMovement()->Launch(JumpVelocity);  // Z축 방향으로 점프

		// 2단 점프 후, 2단 점프 불가능하게 설정
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

	// 근접공격 소리 재생
	if (IsValid(CurrentGameState))
	{
		CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::MeleeAttack);
	}

	// 근접 공격 딜레이
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

	FVector Start = GetActorLocation() + (ForwardVector * (MeleeAttackRadius + 50)); // 공격 시작 위치
	FVector End = Start + (ForwardVector * (MeleeAttackRadius + 50)); // 공격 끝 위치

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
				// 미션 아이템
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
							LaunchVector.Z = 300;
							HitCharacter->LaunchCharacter(LaunchVector, false, false);
						}
					}
				}
			}

			// 데미지를 입힌 액터를 Set에 추가
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
