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

	// 총 쏘기
	virtual bool ActiveWeapon() override;

	// 재장전
	void Reload() override;
	void StopReload();

	// 재장전 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	float ReloadDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	TSubclassOf<AWeapon> Weapon;

	UPROPERTY()
	TObjectPtr<AWeapon> WeaponActor;

	FTimerHandle ReloadDelayHandle;

	bool IsReload;
};
