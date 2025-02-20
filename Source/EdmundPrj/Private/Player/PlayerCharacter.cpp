#include "Player/PlayerCharacter.h"
#include "Player/EdmundPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Player/Weapon.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
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

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AEdmundPlayerController* PlayerController = Cast<AEdmundPlayerController>(GetController()))
		{
			if (PlayerController->FireAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FireAction,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::Fire
				);
			}
		}
	}
}

void APlayerCharacter::Fire(const FInputActionValue& value)
{
	if (IsValid(WeaponActor))
	{
		WeaponActor->Fire();
	}
}