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

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	UAnimMontage* AttackMontage;

	// ���� ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim")
	UAnimMontage* MeleeAttackMontage;

	// ������ �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	UAnimMontage* ReloadMontage;

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

	// �ִ� źȯ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxAmmo;

	// ���� źȯ
	int32 CurrentAmmo;

	UPROPERTY()
	TObjectPtr<AWeapon> WeaponActor;

	FTimerHandle ReloadDelayHandle;
	FTimerHandle MeleeAttackDelayHandle;

	// Notify���� ȣ���ϱ� ���� public
	void AttackTrace() override;
	virtual void ActiveWeapon();
	void PlayReloadSound();
	void SetIsAttack();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ���콺 ȸ��
	void Look(const FInputActionValue& value);

	// ����
	virtual void Attack(const FInputActionValue& value) override;

	// ��������
	void MeleeAttack(const FInputActionValue& value);
	void EndMeleeAttack();

	// ������
	void ReloadAction(const FInputActionValue& value);
	void Reload();
	void StopReload();

	// �� ��/�ƿ�
	void ZoomIn(const FInputActionValue& value);
	void ZoomOut(const FInputActionValue& value);

	// źȯ Setter
	void SetAmmo(int32 NewAmmo);

	// źȯ ���ϱ�
	void AmountAmmo(int32 AmountAmmo);

	// ���� �׼� ���� �� ���� - �ٸ� �׼� �Ұ�
	bool CheckAction() override;

	// ����
	void ActiveDieAction();

	// ��ȭ�س��� �������ͽ��� �ޱ�
	virtual void GetUpgradeStatus() override;

	bool IsAttack;
	bool IsMeleeAttack;
	bool IsReload;
	bool IsZoom;

private:
	float ReloadTimeMultipler;
	FTimerHandle AttackDelayHandle;
};
