#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "System/EnumSet.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

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

	// ������
	void ReloadAction(const FInputActionValue& value);
	void Reload();

	// ��ȣ�ۿ�
	void Interaction(const FInputActionValue& value);

	// �� ��/�ƿ�
	void ZoomIn(const FInputActionValue& value);
	void ZoomOut(const FInputActionValue& value);

	// �ɱ�
	void StartCrouch(const FInputActionValue& value);
	void StopCrouch(const FInputActionValue& value);

	// �ǰ�
	void TakeDamage(float Damage);

	// ����ġ ����
	void AddExp(int32 Exp);

	// ������
	void LevelUp();

	// ���� ü�� Getter
	float GetHP() const;

	// ���� ü�� Setter
	void SetHP(float NewHP);

	// ü�� ȸ��
	void AmountHP(float AmountHP);

	// źȯ Setter
	void SetAmmo(int32 NewAmmo);

	// źȯ ���ϱ�
	void AmountAmmo(int32 AmountAmmo);

	// ���� ��� Getter
	int32 GetGold() const;

	// ��� ȹ��
	void AddGold(int32 Gold);

	// ��ȭ�س��� �������ͽ��� �ޱ�
	void GetUpgradeStatus();

public:
	// ĳ���� Ÿ�� ��ȯ
	ECharacterType getCharacterType();

	// ĳ���� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	ECharacterType CharacterType;

	// ���� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float HP;

	// �ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float MaxHP;

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

	// ���� ��ų
	/*FVector<ActiveSkill> MyActiveSkill;
	FVector<PassiveSkill> MyPassiveSkill;*/

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

	// ���� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CurrentGold;

	// ��Ȱ Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 RevivalCount;

private:
	// �޸��� ��
	bool IsSprint;

	// �ɱ� ��
	bool IsCrouch;

	// ĸ�� ���� <- �ɱ⿡�� ���
	float CapsuleHeight;
};
