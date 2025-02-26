#include "Player/PlayerCharacter.h"
#include "Player/EdmundPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Player/Weapon.h"
#include "System/EdmundGameState.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	ReloadDelay = 2.0f;
	IsReload = false;

	ReloadMontage = nullptr;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	WeaponActor = GetWorld()->SpawnActor<AWeapon>(Weapon);

	if (Weapon)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		WeaponActor->AttachToComponent(GetMesh(), TransformRules, TEXT("WeaponSocket"));
		WeaponActor->SetOwner(this);
	}
}

bool APlayerCharacter::ActiveWeapon()
{
	if (IsValid(WeaponActor))
	{
		return WeaponActor->Fire();
	}

	return false;
}

void APlayerCharacter::Reload()
{
	if (IsReload)
	{
		return;
	}

	Super::Reload();

	if (IsValid(ReloadMontage))
	{
		if (IsValid(ReloadMontage))
		{
			PlayAnimMontage(ReloadMontage);
		}

		IsReload = true;
		bIsReloading = true;

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
	bIsReloading = false;

	if (IsValid(CurrentGameState))
	{
		CurrentGameState->NotifyPlayerAmmo(MaxAmmo, CurrentAmmo);
	}
}
