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

	// ���ݷ� Getter
	float GetAttackDamage() const;

	// ���ݷ� Setter
	void SetAttackDamage(float NewAttackDamage);

	// ���ݷ� Getter
	float GetAttackDelay() const;

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

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �̵�
	void Move(const FInputActionValue& value);

	// ���콺 ȸ��
	virtual void Look(const FInputActionValue& value);

	// ����
	void StartJump(const FInputActionValue& value);
	void StopJump(const FInputActionValue& value);
	
	// �޸���
	void StartSprint(const FInputActionValue& value);
	void StopSprint(const FInputActionValue& value);

	// ��ȣ�ۿ�
	void Interaction(const FInputActionValue& value);

	// �ɱ�
	void StartCrouch(const FInputActionValue& value);
	void StopCrouch(const FInputActionValue& value);

	// ��ȣ�ۿ�
	void PauseAction(const FInputActionValue& value);

	// �ǰ�
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	// ��ȭ�س��� �������ͽ��� �ޱ�
	void GetUpgradeStatus();

	// ����
	virtual void ActiveDieAction();

public:
	// ĳ���� Ÿ�� ��ȯ
	ECharacterType GetCharacterType();

	virtual void AttackTrace();

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

	// ���� ������ (����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float AttackDelay;

	// ġ��Ÿ Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CriticalProb;

	// ġ��Ÿ Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CriticalMultiplier;

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

	// �ǰ� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	UAnimMontage* HitActionMontage;

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	UAnimMontage* DieActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	UAudioComponent* CurrentAudioComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> EvasionSuccessSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> RevivalSuccessSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> DeathSound;

private:
	// ĸ�� ���� <- �ɱ⿡�� ���
	float CapsuleHeight;

protected:
	bool IsDie;

	AEdmundGameState* CurrentGameState;

	virtual bool CheckAction();
};
