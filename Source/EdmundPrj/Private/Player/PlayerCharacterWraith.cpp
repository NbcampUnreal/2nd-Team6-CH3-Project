#include "Player/PlayerCharacterWraith.h"
#include "Player/EdmundPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Player/Weapon.h"
#include "System/EdmundGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/DataStructure/ShopCatalogRow.h"
#include "Components/StaticMeshComponent.h"

APlayerCharacterWraith::APlayerCharacterWraith()
{
	PrimaryActorTick.bCanEverTick = false;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BulletMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	ChargingScaleMultiplier = FVector(0.03f, 0.03f, 0.03f);
	ChargingLocationMultiplier = FVector(0.0f, 2.0f, 0.0f);
	ChargingAttackMultiplier = 0.05f;
	MaxCharging = 3.0f;

	ReloadDelay = 2.0f;
	MeleeAttackDelay = 0.7f;
	MeleeAttackRadius = 100.0f;
	MeleeAttackPushStrength = 1000.0f;
	MeleeAttackForwardOffset = 50.0f;

	ChargingDelay = 0.1f;
	AttackMultipler = 0.5f;

	CurrentAmmo = MaxAmmo = 20;

	ZoomInLength = 100.0f;
	ZoomOutLength = 200.0f;
	ZoomMouseMoveMultipler = 0.5f;

	IsMeleeAttack = false;
	IsAttack = false;
	IsReload = false;

	ReloadTimeMultipler = 1.0f;
}

void APlayerCharacterWraith::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
	ZoomOutLength = SpringArmComp->TargetArmLength;

	WeaponActor = GetWorld()->SpawnActor<AWeapon>(Weapon);

	check(Weapon && CurrentGameState);

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	WeaponActor->AttachToComponent(GetMesh(), TransformRules, TEXT("WeaponSocket"));
	WeaponActor->SetOwner(this);

	CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
}

float APlayerCharacterWraith::GetAttackDamage() const
{
	float Damage = AttackDamage;

	int32 DamageProb = FMath::RandRange(1, 100);

	// 크리티컬
	if (DamageProb <= CriticalProb)
	{
		Damage *= CriticalMultiplier;
	}

	return Damage * AttackMultipler;
}

void APlayerCharacterWraith::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AEdmundPlayerController* PlayerController = Cast<AEdmundPlayerController>(GetController()))
		{
			if (PlayerController->AttackAction)
			{
				EnhancedInput->BindAction(PlayerController->AttackAction, ETriggerEvent::Started, this, &APlayerCharacterWraith::StartAttack);
			}

			if (PlayerController->AttackAction)
			{
				EnhancedInput->BindAction(PlayerController->AttackAction, ETriggerEvent::Completed, this, &APlayerCharacterWraith::EndAttack);
			}

			if (PlayerController->MeleeAttackAction)
			{
				EnhancedInput->BindAction(PlayerController->MeleeAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacterWraith::MeleeAttack);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(PlayerController->ReloadAction, ETriggerEvent::Triggered, this, &APlayerCharacterWraith::ReloadAction);
			}

			if (PlayerController->ZoomAction)
			{
				EnhancedInput->BindAction(PlayerController->ZoomAction, ETriggerEvent::Started, this, &APlayerCharacterWraith::ZoomIn);
				EnhancedInput->BindAction(PlayerController->ZoomAction, ETriggerEvent::Completed, this, &APlayerCharacterWraith::ZoomOut);
			}
		}
	}
}

void APlayerCharacterWraith::Look(const FInputActionValue& value)
{
	if (IsDie)
	{
		return;
	}

	FVector2D LookInput = value.Get<FVector2D>();

	// 줌상태에서 마우스 감도 낮추기
	if (IsZoom)
	{
		LookInput *= ZoomMouseMoveMultipler;
	}

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacterWraith::StartAttack(const FInputActionValue& value)
{
	check(WeaponActor);

	if (CurrentAmmo <= 0 || CheckAction())
	{
		return;
	}

	if (IsValid(ChargeMontage))
	{
		PlayAnimMontage(ChargeMontage);
	}

	IsAttack = true;

	BulletScale = BulletMesh->GetRelativeScale3D();
	BulletLocation = BulletMesh->GetRelativeLocation();
	SaveAttackMultipler = AttackMultipler;

	BulletMesh->SetVisibility(true);

	GetWorld()->GetTimerManager().SetTimer(
		CharingBulletTimerHandle,
		this,
		&ThisClass::UpScale,
		ChargingDelay,
		true
	);
}

void APlayerCharacterWraith::EndAttack(const FInputActionValue& value)
{
	if (!IsAttack)
	{
		return;
	}

	NormalizeScale();
}

bool APlayerCharacterWraith::ActiveWeapon() const
{
	if (IsValid(WeaponActor))
	{
		return WeaponActor->Fire(AttackDelay);
	}

	return false;
}

void APlayerCharacterWraith::MeleeAttack(const FInputActionValue& value)
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

void APlayerCharacterWraith::EndMeleeAttack()
{
	IsMeleeAttack = false;
}

void APlayerCharacterWraith::AttackTrace()
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

FVector APlayerCharacterWraith::GetBulletScale() const
{
	check(BulletMesh);
	return BulletMesh->GetRelativeScale3D();
}

void APlayerCharacterWraith::ReloadAction(const FInputActionValue& value)
{
	if (CheckAction() || IsDie || CurrentAmmo == MaxAmmo)
	{
		return;
	}

	Reload();
}

void APlayerCharacterWraith::Reload()
{
	CurrentAmmo = MaxAmmo;

	check(CurrentGameState);

	CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Reload);

	if (IsValid(ReloadMontage))
	{
		if (IsValid(ReloadMontage))
		{
			PlayAnimMontage(ReloadMontage, ReloadTimeMultipler);
		}

		IsReload = true;

		GetWorld()->GetTimerManager().SetTimer(
			ReloadDelayHandle,
			this,
			&ThisClass::StopReload,
			ReloadDelay,
			false
		);
	}
}

void APlayerCharacterWraith::StopReload()
{
	IsReload = false;

	check(CurrentGameState);

	CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);

}

void APlayerCharacterWraith::ZoomIn(const FInputActionValue& value)
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
	SpringArmComp->TargetArmLength = ZoomInLength;
}

void APlayerCharacterWraith::ZoomOut(const FInputActionValue& value)
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
	SpringArmComp->TargetArmLength = ZoomOutLength;
}

void APlayerCharacterWraith::SetAmmo(int32 NewAmmo)
{
	CurrentAmmo = FMath::Min(MaxAmmo, NewAmmo);

	check(CurrentGameState);

	CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);

}

void APlayerCharacterWraith::AmountAmmo(int32 AmountAmmo)
{
	CurrentAmmo = FMath::Min(MaxAmmo, CurrentAmmo + AmountAmmo);

	check(CurrentGameState);

	CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
}

bool APlayerCharacterWraith::CheckAction()
{
	return IsDie || IsAttack || IsMeleeAttack || IsReload;
}

void APlayerCharacterWraith::ActiveDieAction()
{
	// 만약 줌상태라면
	SpringArmComp->TargetArmLength = ZoomOutLength;

	if (IsValid(DieActionMontage))
	{
		PlayAnimMontage(DieActionMontage);
	}
}

void APlayerCharacterWraith::GetUpgradeStatus()
{
	Super::GetUpgradeStatus();

	if (!IsValid(CurrentGameState))
	{
		return;
	}

	TArray<FShopCatalogRow*> ShopStatusList = CurrentGameState->GetPlayerAdvancedData();

	// MaxAmmo
	MaxAmmo = MaxAmmo + ShopStatusList[10]->CurrentLevel * ShopStatusList[10]->AdvanceValue;

	// ReloadTime
	ReloadTimeMultipler = ReloadTimeMultipler * (1.0f + ShopStatusList[11]->CurrentLevel * ShopStatusList[11]->AdvanceValue);
	ReloadDelay *= (1.0f - ShopStatusList[11]->CurrentLevel * ShopStatusList[11]->AdvanceValue / 2);
}

void APlayerCharacterWraith::UpScale()
{
	BulletMesh->SetRelativeScale3D(BulletMesh->GetRelativeScale3D() + ChargingScaleMultiplier);
	BulletMesh->AddRelativeLocation(ChargingLocationMultiplier);
	AttackMultipler += ChargingAttackMultiplier;

	if (BulletMesh->GetRelativeScale3D().X >= MaxCharging)
	{
		NormalizeScale();
	}
}

void APlayerCharacterWraith::NormalizeScale()
{
	if (ActiveWeapon())
	{
		check(CurrentGameState);

		CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Attack);

		if (IsValid(AttackMontage))
		{
			PlayAnimMontage(AttackMontage);
		}

		Super::Attack(FInputActionValue());
		CurrentAmmo--;
	}

	GetWorld()->GetTimerManager().ClearTimer(CharingBulletTimerHandle);

	BulletMesh->SetVisibility(false);
	BulletMesh->SetRelativeScale3D(BulletScale);
	BulletMesh->SetRelativeLocation(BulletLocation);

	IsAttack = false;

	AttackMultipler = SaveAttackMultipler;

	check(CurrentGameState);

	CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);

	if (CurrentAmmo <= 0)
	{
		Reload();
	}
}
