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

	virtual void ActiveWeapon();

	// Notify���� ȣ��
	void AttackTrace() override;
	void SetJumpCount();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void StartJump(const FInputActionValue& value) override;

	UFUNCTION()
	void TriggerJump(const FInputActionValue& value);

	// ����
	virtual void Attack(const FInputActionValue& value) override;

	// ��������
	void MeleeAttack(const FInputActionValue& value);
	void EndMeleeAttack();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> Weapon;

	UPROPERTY()
	TObjectPtr<AWeapon> WeaponActor;

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	// ���� ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MeleeAttackMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 AttackCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float HoveringZSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float HoveringDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	int32 JumpCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float DoubleJumpHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float MeleeAttackDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float MeleeAttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float MeleeAttackPushStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float MeleeAttackForwardOffset;

	bool IsAttack;
	bool IsHover;
	bool IsMeleeAttack;

	FTimerHandle HoveringDelayHandle;
	FTimerHandle AttackDelayHandle;
	FTimerHandle MeleeAttackDelayHandle;

	void SetIsHover();
	void SetIsAttack();
	bool CheckAction() override;
};
