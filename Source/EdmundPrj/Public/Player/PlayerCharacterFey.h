#pragma once

#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
#include "PlayerCharacterFey.generated.h"

class AWeapon;

UCLASS()
class EDMUNDPRJ_API APlayerCharacterFey : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacterFey();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void StartJump(const FInputActionValue& value) override;

	UFUNCTION()
	void TriggerJump(const FInputActionValue& value);

	// ��������
	void MeleeAttack(const FInputActionValue& value);
	void EndMeleeAttack();

	virtual void ActiveWeapon();

	// Notify���� ȣ���ϱ� ���� public
	void AttackTrace() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	TSubclassOf<AWeapon> Weapon;

	UPROPERTY()
	TObjectPtr<AWeapon> WeaponActor;

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim")
	UAnimMontage* AttackMontage;

	// ���� ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim")
	UAnimMontage* MeleeAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> MeleeAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float HoveringZSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	int32 JumpCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float DoubleJumpHeight;

protected:
	// ����
	virtual void Attack(const FInputActionValue& value) override;

private:
	bool IsAttack;

	bool IsHover;
	float HoveringDelay;

	bool IsMeleeAttack;
	float MeleeAttackDelay;
	float MeleeAttackRadius;
	float MeleeAttackPushStrength;

	FTimerHandle HoveringDelayHandle;
	FTimerHandle AttackDelayHandle;
	FTimerHandle MeleeAttackDelayHandle;

	void SetIsHover();
	void SetIsAttack();

	// ���� �׼� ���� �� ���� - �ٸ� �׼� �Ұ�
	bool CheckAction() override;
};
