#pragma once

#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
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
	virtual bool ActiveWeapon() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	TSubclassOf<AWeapon> Weapon;

	UPROPERTY()
	TObjectPtr<AWeapon> WeaponActor;
};
