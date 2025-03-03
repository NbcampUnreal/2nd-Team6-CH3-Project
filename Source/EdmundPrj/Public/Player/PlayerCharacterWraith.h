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

	// ���ݷ� Getter
	virtual float GetAttackDamage() const override;

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim")
	UAnimMontage* ChargeMontage;

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
	TObjectPtr<USoundBase> FireSound;			// �ѼҸ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> MeleeAttackSound;	// �������� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> ReloadSound;

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

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ���콺 ȸ��
	void Look(const FInputActionValue& value);

	// ����
	virtual void StartAttack(const FInputActionValue& value);
	virtual void Attack(const FInputActionValue& value) override;
	virtual void EndAttack(const FInputActionValue& value);

	virtual bool ActiveWeapon();

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
	float AttackMultipler;
	float SaveAttackMultipler;

	FVector BulletScale;
	FVector BulletLocation;

	FTimerHandle CharingBulletTimerHandle;

	void UpScale();
	void NormalizeScale();
};
