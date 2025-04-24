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

	// Notify에서 호출
	void AttackTrace() override;

	// 총알 스케일 Getter
	FVector GetBulletScale() const;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 마우스 회전
	void Look(const FInputActionValue& value);

	// 공격
	void StartAttack(const FInputActionValue& value);
	void Attack(const FInputActionValue& value) override;
	void EndAttack(const FInputActionValue& value);

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
	void SetAmmo(const int32 NewAmmo);

	// 탄환 더하기
	void AmountAmmo(const int32 AmountAmmo);

private:
	void UpScale();
	void NormalizeScale();

	// 강화해놓은 스테이터스값 받기
	virtual void GetUpgradeStatus() override;

	// 현재 액션 실행 중 인지 - 다른 액션 불가
	bool CheckAction() override;

	// 죽음
	void ActiveDieAction();

	bool ActiveWeapon() const;

	// 공격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ChargeMontage = nullptr;

	// 공격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	// 근접 공격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MeleeAttackMontage = nullptr;

	// 재장전 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ReloadMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BulletMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon", meta = (AllowPrivateAccess = "true"))
	float ReloadDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float MeleeAttackDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float MeleeAttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float MeleeAttackPushStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float ZoomMouseMoveMultipler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> Weapon;

	UPROPERTY()
	TObjectPtr<AWeapon> WeaponActor;

	// 최대 탄환
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 MaxAmmo;

	// 현재 탄환
	int32 CurrentAmmo;

	bool IsAttack;
	bool IsMeleeAttack;
	bool IsReload;
	bool IsZoom;

	float ReloadTimeMultipler;
	float AttackMultipler;
	float SaveAttackMultipler;

	FVector BulletScale;
	FVector BulletLocation;

	FTimerHandle ReloadDelayHandle;
	FTimerHandle MeleeAttackDelayHandle;
	FTimerHandle CharingBulletTimerHandle;
};
