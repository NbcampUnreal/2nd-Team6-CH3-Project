#include "Player/PlayerCharacter.h"
#include "Player/EdmundPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Player/Weapon.h"
#include "System/EdmundGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/DataStructure/ShopCatalogRow.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	ReloadDelay = 2.0f;
	MeleeAttackDelay = 0.7f;
	MeleeAttackRadius = 100.0f;
	MeleeAttackPushStrength = 1000.0f;

	ReloadMontage = nullptr;

	AttackMontage = nullptr;
	MeleeAttackMontage = nullptr;

	CurrentAmmo = MaxAmmo = 20;
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
	WeaponActor = GetWorld()->SpawnActor<AWeapon>(Weapon);

	if (Weapon)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		WeaponActor->AttachToComponent(GetMesh(), TransformRules, TEXT("WeaponSocket"));
		WeaponActor->SetOwner(this);
	}

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
	}
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
				EnhancedInput->BindAction(
					PlayerController->MeleeAttackAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::MeleeAttack
				);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::ReloadAction
				);
			}

			if (PlayerController->ZoomAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ZoomAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::ZoomIn
				);

				EnhancedInput->BindAction(
					PlayerController->ZoomAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::ZoomOut
				);
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

	if (CurrentAmmo <= 0 || CheckAction())
	{
		return;
	}

	if (ActiveWeapon())
	{
		if (IsValid(CurrentGameState))
		{
			CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Attack);
		}

		//// �ѼҸ� ���
		//if (FireSound)
		//{
		//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		//	CurrentAudioComp->SetSound(FireSound);
		//	CurrentAudioComp->Play();
		//}

		if (IsValid(AttackMontage))
		{
			PlayAnimMontage(AttackMontage);
		}

		Super::Attack(value);
		CurrentAmmo--;
	}

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
	}

	if (CurrentAmmo <= 0)
	{
		Reload();
	}
}

bool APlayerCharacter::ActiveWeapon()
{
	if (IsValid(WeaponActor))
	{
		return WeaponActor->Fire(AttackDelay);
	}

	return false;
}

void APlayerCharacter::MeleeAttack(const FInputActionValue& value)
{
	if (CheckAction() || IsDie)
	{
		return;
	}

	if (IsValid(MeleeAttackMontage))
	{
		PlayAnimMontage(MeleeAttackMontage);
	}

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::MeleeAttack);
	}

	//// �������� �Ҹ� ���
	//if (MeleeAttackSound)
	//{
	//	CurrentAudioComp->SetSound(MeleeAttackSound);
	//	CurrentAudioComp->Play();
	//}

	// ���� ���� ������
	GetWorld()->GetTimerManager().SetTimer(
		MeleeAttackDelayHandle,
		this,
		&APlayerCharacter::EndMeleeAttack,
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

	FVector Start = GetActorLocation() + (ForwardVector * (MeleeAttackRadius + 50)); // ���� ���� ��ġ
	FVector End = Start + (ForwardVector * (MeleeAttackRadius + 50)); // ���� �� ��ġ

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

	// ��ü ���̰� �ϱ�
	if (GEngine)
	{
		DrawDebugSphere(
			GetWorld(),
			Start,
			Radius,
			12,
			FColor::Red,        // ����
			false,              // ���Ӽ� (���� �� ��� ǥ������ ����)
			1.0f                // ���� �ð�
		);
	}

	// �������� ���� ���͸� ������ Set (�ߺ� ����)
	// Set�� ������ ���������ѹ��� ������ ������ �޴� ���� �߻�
	TSet<AActor*> DamagedActors;

	if (bHit)
	{
		// ���� �浹 ��ü�� �ִٸ�
		for (const FHitResult& Hit : HitResults)
		{
			// �浹�� ��ü�� �ִٸ�
			AActor* HitActor = Hit.GetActor();

			if (!DamagedActors.Contains(HitActor) && HitActor && (HitActor->ActorHasTag("MissionItem") || HitActor->ActorHasTag("Monster")))
			{
				// �̼� �������� ������ �ֱ�
				if (HitActor->ActorHasTag("MissionItem"))
				{
					UGameplayStatics::ApplyDamage(
						HitActor,
						30.0f,	// �����ʿ�
						nullptr,
						this,
						UDamageType::StaticClass()
					);
				}

				// ���ʹ� ��ġ��
				if (HitActor->ActorHasTag("Monster"))
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

			// �������� ���� ���͸� Set�� �߰�
			DamagedActors.Add(HitActor);
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
	CurrentAmmo = MaxAmmo;

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->PlayPlayerSound(CurrentAudioComp, ESoundType::Reload);
	}

	/*if (IsValid(ReloadSound))
	{
		CurrentAudioComp->SetSound(ReloadSound);
		CurrentAudioComp->Play();
	}*/

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
			&APlayerCharacter::StopReload,
			ReloadDelay,
			false
		);
	}
}

void APlayerCharacter::StopReload()
{
	IsReload = false;

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
	}
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
	SpringArmComp->TargetArmLength = 100;
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
	SpringArmComp->TargetArmLength = 200;
}

void APlayerCharacter::SetAmmo(int32 NewAmmo)
{
	CurrentAmmo = FMath::Min(MaxAmmo, NewAmmo);

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
	}
}

void APlayerCharacter::AmountAmmo(int32 AmountAmmo)
{
	CurrentAmmo = FMath::Min(MaxAmmo, CurrentAmmo + AmountAmmo);

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
	}
}

bool APlayerCharacter::CheckAction()
{
	return IsDie || IsAttack || IsMeleeAttack || IsReload;
}

void APlayerCharacter::ActiveDieAction()
{
	// ���� �ܻ��¶��
	IsZoom = false;
	SpringArmComp->TargetArmLength = 300;

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
