#include "Player/PlayerCharacter.h"
#include "Player/EdmundPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Player/Weapon.h"
#include "System/EdmundGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/DataStructure/ShopCatalogRow.h"

APlayerCharacter::APlayerCharacter() : Super()
{
	PrimaryActorTick.bCanEverTick = false;

	ReloadDelay = 2.0f;
	MeleeAttackDelay = 0.7f;
	MeleeAttackRadius = 100.0f;
	MeleeAttackPushStrength = 1000.0f;
	MeleeAttackForwardOffset = 50.0f;

	ReloadMontage = nullptr;

	AttackMontage = nullptr;
	MeleeAttackMontage = nullptr;

	CurrentAmmo = MaxAmmo = 20;

	ZoomInLength = 100.0f;
	ZoomOutLength = 200.0f;
	ZoomMouseMoveMultipler = 0.5f;

	IsMeleeAttack = false;
	IsAttack = false;
	IsReload = false;

	ReloadTimeMultipler = 1.0f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
	ZoomOutLength = SpringArmComp->TargetArmLength;

	check(Weapon);

	WeaponActor = GetWorld()->SpawnActor<AWeapon>(Weapon);

	if (IsValid(WeaponActor))
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		WeaponActor->AttachToComponent(GetMesh(), TransformRules, TEXT("WeaponSocket"));
		WeaponActor->SetOwner(this);
	}

	check(CurrentGameState);

	CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AEdmundPlayerController* PlayerController = Cast<AEdmundPlayerController>(GetController()))
		{
			if (PlayerController->MeleeAttackAction)
			{
				EnhancedInput->BindAction(PlayerController->MeleeAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MeleeAttack);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(PlayerController->ReloadAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ReloadAction);
			}

			if (PlayerController->ZoomAction)
			{
				EnhancedInput->BindAction(PlayerController->ZoomAction, ETriggerEvent::Started, this, &APlayerCharacter::ZoomIn);
				EnhancedInput->BindAction(PlayerController->ZoomAction, ETriggerEvent::Completed, this, &APlayerCharacter::ZoomOut);
			}
		}
	}
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	if (IsDie)
	{
		return;
	}

	FVector2D LookInput = value.Get<FVector2D>();

	// �ܻ��¿��� ���콺 ���� ���߱�
	if (IsZoom)
	{
		LookInput *= ZoomMouseMoveMultipler;
	}

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::Attack(const FInputActionValue& value)
{
	if (CurrentAmmo <= 0 || CheckAction() || IsAttack)
	{
		return;
	}

	Super::Attack(value);

	IsAttack = true;

	GetWorld()->GetTimerManager().SetTimer(
		AttackDelayHandle,
		this,
		&ThisClass::SetIsAttack,
		AttackDelay,
		false
	);

	if (IsValid(AttackMontage))
	{
		PlayAnimMontage(AttackMontage);
	}
}

void APlayerCharacter::ActiveWeapon()
{
	check(WeaponActor && CurrentGameState);

	if (WeaponActor->Fire(AttackDelay))
	{
		CurrentAmmo--;
		CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
		CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Attack);
	}

	if (CurrentAmmo <= 0)
	{
		Reload();
	}
}

void APlayerCharacter::MeleeAttack(const FInputActionValue& value)
{
	if (CheckAction() || IsDie)
	{
		return;
	}

	IsAttack = false;

	if (IsValid(MeleeAttackMontage))
	{
		PlayAnimMontage(MeleeAttackMontage);
	}

	check(CurrentGameState);

	CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::MeleeAttack);

	// ���� ���� �����
	GetWorld()->GetTimerManager().SetTimer(
		MeleeAttackDelayHandle,
		this,
		&ThisClass::EndMeleeAttack,
		MeleeAttackDelay,
		false
	);

	IsMeleeAttack = true;
}

void APlayerCharacter::EndMeleeAttack()
{
	IsMeleeAttack = false;
}

void APlayerCharacter::AttackTrace()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FRotator ControlRotation = PlayerController->GetControlRotation();
	FVector ForwardVector = ControlRotation.Vector();

	FVector Start = GetActorLocation() + (ForwardVector * (MeleeAttackRadius + MeleeAttackForwardOffset)); // ���� ���� ��ġ
	FVector End = Start + (ForwardVector * (MeleeAttackRadius + MeleeAttackForwardOffset)); // ���� �� ��ġ

	// ���� ���� ������ �浹 üũ
	float Radius = MeleeAttackRadius;
	TArray<FHitResult> HitResults;

	// Ʈ���̽� ����
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // �ڽ��� �����ϵ��� ����

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,               // ���� ��ġ
		End,                 // �� ��ġ
		FQuat::Identity,     // ȸ���� (ȸ�� ����)
		ECollisionChannel::ECC_OverlapAll_Deprecated, // �浹 ä��
		FCollisionShape::MakeSphere(Radius), // ���� ���� (��ü ���)
		QueryParams
	);

	if (!bHit)
	{
		return;
	}

	// ���� �浹 ��ü�� �ִٸ�
	for (const FHitResult& Hit : HitResults)
	{
		// �浹�� ��ü�� �ִٸ�
		AActor* HitActor = Hit.GetActor();

		if (!IsValid(HitActor) || HitActor->ActorHasTag("Boss"))
		{
			continue;
		}

		// �̼� ������ ������ �ֱ�
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

		// ���ʹ� ��ġ��
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

void APlayerCharacter::ReloadAction(const FInputActionValue& value)
{
	if (CheckAction() || IsDie || CurrentAmmo == MaxAmmo)
	{
		return;
	}

	Reload();
}

void APlayerCharacter::Reload()
{
	IsReload = true;

	CurrentAmmo = MaxAmmo;

	if (IsValid(ReloadMontage))
	{
		PlayAnimMontage(ReloadMontage, ReloadTimeMultipler);
	}
}

void APlayerCharacter::StopReload()
{
	IsReload = false;

	check(CurrentGameState);

	CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
}

void APlayerCharacter::ZoomIn(const FInputActionValue& value)
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

void APlayerCharacter::ZoomOut(const FInputActionValue& value)
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

void APlayerCharacter::SetAmmo(const int32 NewAmmo)
{
	CurrentAmmo = FMath::Min(MaxAmmo, NewAmmo);

	check(CurrentGameState)

	CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
}

void APlayerCharacter::AmountAmmo(const int32 AmountAmmo)
{
	CurrentAmmo = FMath::Min(MaxAmmo, CurrentAmmo + AmountAmmo);

	check(CurrentGameState);

	CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
}

bool APlayerCharacter::CheckAction()
{
	return IsDie || IsMeleeAttack || IsReload;
}

void APlayerCharacter::ActiveDieAction()
{
	// ���� �ܻ��¶��
	SpringArmComp->TargetArmLength = ZoomOutLength;

	if (IsValid(DieActionMontage))
	{
		PlayAnimMontage(DieActionMontage);
	}
}

void APlayerCharacter::GetUpgradeStatus()
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

void APlayerCharacter::SetIsAttack()
{
	IsAttack = false;
}

void APlayerCharacter::PlayReloadSound()
{
	check(CurrentGameState);
	
	CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Reload);

	GetWorld()->GetTimerManager().SetTimer(
		ReloadDelayHandle,
		this,
		&ThisClass::StopReload,
		ReloadDelay,
		false
	);
}
