#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "System/EnumSet.h"
#include "Components/AudioComponent.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class AEdmundGameState;

UCLASS()
class EDMUNDPRJ_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
	UCameraComponent* CameraComp;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �̵�
	void Move(const FInputActionValue& value);

	// ���콺 ȸ��
	void Look(const FInputActionValue& value);

	// ����
	void StartJump(const FInputActionValue& value);
	void StopJump(const FInputActionValue& value);
	
	// �޸���
	void StartSprint(const FInputActionValue& value);
	void StopSprint(const FInputActionValue& value);

	// ����
	void Attack(const FInputActionValue& value);
	virtual bool ActiveWeapon();

	// ��������
	void MeleeAttack(const FInputActionValue& value);
	void EndMeleeAttack();

	// ������
	void ReloadAction(const FInputActionValue& value);
	virtual void Reload();

	// ��ȣ�ۿ�
	void Interaction(const FInputActionValue& value);

	// �� ��/�ƿ�
	void ZoomIn(const FInputActionValue& value);
	void ZoomOut(const FInputActionValue& value);

	// �ɱ�
	void StartCrouch(const FInputActionValue& value);
	void StopCrouch(const FInputActionValue& value);

	// �ǰ�
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	// ����ġ ����
	void AddExp(int32 Exp);

	// ������
	void LevelUp();

	// ���� ü�� Getter
	int32 GetHP() const;

	// ���� ü�� Setter
	void SetHP(int32 NewHP);

	// ü�� ȸ��
	void AmountHP(int32 AmountHP);

	// źȯ Setter
	void SetAmmo(int32 NewAmmo);

	// źȯ ���ϱ�
	void AmountAmmo(int32 AmountAmmo);

	// ��ȭ�س��� �������ͽ��� �ޱ�
	void GetUpgradeStatus();

	// ����
	void ActiveDieAction();

public:
	// ĳ���� Ÿ�� ��ȯ
	ECharacterType GetCharacterType();

	void MeleeAttackTrace();

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* AttackMontage;

	// ĳ���� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	ECharacterType CharacterType;

	// ���� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 HP;

	// �ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxHP;

	// �ȱ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float WalkSpeed;

	// �޸��� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float SprintSpeed;

	// �ɾƼ� �̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float CrouchMoveSpeed;

	// ���ݷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float AttackDamage;

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 Defense;

	// ���� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float AttackSpeed;

	// �ִ� źȯ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxAmmo;

	// ���� źȯ
	int32 CurrentAmmo;

	// ġ��Ÿ Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CriticalProb;

	// ȸ�� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 EvasionProb;

	// ���� ����ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CurrentExp;

	// �ִ� ����ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxExp;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CurrentLevel;

	// �ִ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxLevel;

	// ��Ȱ Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 RevivalCount;

	// �޸��� ��
	bool IsSprint;

	// �ɱ� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	bool IsCrouch;

	// ������ �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	UAnimMontage* MeleeAttackMontage;

	// ������ �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float MeleeAttackDelay;

	// �ǰ� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	UAnimMontage* HitActionMontage;

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	UAnimMontage* DieActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> MeleeAttackSound;	// �������� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> ReloadSound;			// ������ ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	UAudioComponent* CurrentAudioComp;

private:
	// ĸ�� ���� <- �ɱ⿡�� ���
	float CapsuleHeight;

	FTimerHandle MeleeAttackDelayHandle;

protected:
	bool bIsMeleeAttack;
	bool bIsAttack;
	bool bIsReloading;
	bool IsDie;
	bool IsZoom;

	AEdmundGameState* CurrentGameState;

	bool CheckAction();
};
