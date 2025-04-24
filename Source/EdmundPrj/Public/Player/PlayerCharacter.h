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

	// 탄환 Setter
	void SetAmmo(const int32 NewAmmo);

	// 탄환 더하기
	void AmountAmmo(const int32 AmountAmmo);

	// Notify에서 호출
	void AttackTrace() override;
	virtual void ActiveWeapon();
	void PlayReloadSound();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 마우스 회전
	void Look(const FInputActionValue& value);

	// 공격
	virtual void Attack(const FInputActionValue& value) override;

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

	// 현재 액션 실행 중 인지 - 다른 액션 불가
	bool CheckAction() override;

	// 죽음
	void ActiveDieAction();

	// 강화해놓은 스테이터스값 받기
	virtual void GetUpgradeStatus() override;

private:
	void SetIsAttack();
	
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

	// 최대 탄환
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 MaxAmmo;

	// 현재 탄환
	int32 CurrentAmmo;

	UPROPERTY()
	TObjectPtr<AWeapon> WeaponActor;

	float ReloadTimeMultipler;

	FTimerHandle ReloadDelayHandle;
	FTimerHandle MeleeAttackDelayHandle;
	FTimerHandle AttackDelayHandle;

	bool IsMeleeAttack;
	bool IsReload;
	bool IsZoom;
};
