#pragma once

#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
#include "PlayerCharacterWraith.generated.h"

class AWeapon;
struct FInputActionValue;
class UStaticMeshComponent;

UCLASS()
class EDMUNDPRJ_API APlayerCharacterWraith : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacterWraith();

	virtual void BeginPlay() override;

	// 공격력 Getter
	virtual float GetAttackDamage() const override;

	// 공격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim")
	UAnimMontage* ChargeMontage;

	// 공격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	UAnimMontage* AttackMontage;

	// 근접 공격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim")
	UAnimMontage* MeleeAttackMontage;

	// 재장전 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	float ReloadDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float MeleeAttackDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float MeleeAttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float MeleeAttackPushStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float ZoomMouseMoveMultipler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	TSubclassOf<AWeapon> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> FireSound;			// 총소리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> MeleeAttackSound;	// 근접공격 사운드

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> ReloadSound;

	// 최대 탄환
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxAmmo;

	// 현재 탄환
	int32 CurrentAmmo;

	UPROPERTY()
	TObjectPtr<AWeapon> WeaponActor;

	FTimerHandle ReloadDelayHandle;
	FTimerHandle MeleeAttackDelayHandle;

	// Notify에서 호출하기 위해 public
	void AttackTrace() override;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 마우스 회전
	void Look(const FInputActionValue& value);

	// 공격
	virtual void StartAttack(const FInputActionValue& value);
	virtual void Attack(const FInputActionValue& value) override;
	virtual void EndAttack(const FInputActionValue& value);

	virtual bool ActiveWeapon();

	// 근접공격
	void MeleeAttack(const FInputActionValue& value);
	void EndMeleeAttack();

	// 재장전
	void ReloadAction(const FInputActionValue& value);
	void Reload();
	void StopReload();

	// 줌 인/아웃
	void ZoomIn(const FInputActionValue& value);
	void ZoomOut(const FInputActionValue& value);

	// 탄환 Setter
	void SetAmmo(int32 NewAmmo);

	// 탄환 더하기
	void AmountAmmo(int32 AmountAmmo);

	// 현재 액션 실행 중 인지 - 다른 액션 불가
	bool CheckAction() override;

	// 죽음
	void ActiveDieAction();

	// 강화해놓은 스테이터스값 받기
	virtual void GetUpgradeStatus() override;

	bool IsAttack;
	bool IsMeleeAttack;
	bool IsReload;
	bool IsZoom;

private:
	float ReloadTimeMultipler;
	float AttackMultipler;
	float SaveAttackMultipler;

	FVector BulletScale;
	FVector BulletLocation;

	FTimerHandle CharingBulletTimerHandle;

	void UpScale();
	void NormalizeScale();
};
