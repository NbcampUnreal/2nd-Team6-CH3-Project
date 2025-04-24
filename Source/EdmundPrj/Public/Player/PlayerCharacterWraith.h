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

	// Notify���� ȣ��
	void AttackTrace() override;

	// �Ѿ� ������ Getter
	FVector GetBulletScale() const;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ���콺 ȸ��
	void Look(const FInputActionValue& value);

	// ����
	void StartAttack(const FInputActionValue& value);
	void Attack(const FInputActionValue& value) override;
	void EndAttack(const FInputActionValue& value);

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
	void SetAmmo(const int32 NewAmmo);

	// źȯ ���ϱ�
	void AmountAmmo(const int32 AmountAmmo);

private:
	void UpScale();
	void NormalizeScale();

	// ��ȭ�س��� �������ͽ��� �ޱ�
	virtual void GetUpgradeStatus() override;

	// ���� �׼� ���� �� ���� - �ٸ� �׼� �Ұ�
	bool CheckAction() override;

	// ����
	void ActiveDieAction();

	bool ActiveWeapon() const;

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ChargeMontage = nullptr;

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	// ���� ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MeleeAttackMontage = nullptr;

	// ������ �ִϸ��̼�
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

	// �ִ� źȯ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 MaxAmmo;

	// ���� źȯ
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
