#pragma once

#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
#include "System/EnumSet.h"
#include "PlayerCharacter.generated.h"

class AWeapon;
struct FInputActionValue;

UCLASS()
class EDMUNDPRJ_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();
	virtual void BeginPlay() override;

	// ÃÑ ½î±â
	void Fire(const FInputActionValue& value);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	TSubclassOf<AWeapon> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	ECharacterType CharacterType;

	UPROPERTY()
	TObjectPtr<AWeapon> WeaponActor;
};
