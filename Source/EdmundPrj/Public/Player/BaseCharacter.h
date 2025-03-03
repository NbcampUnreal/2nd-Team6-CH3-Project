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
class USkillManager;
class UTimerSkillSpawnManagerComponent;
class UPassiveSkillManager;
class UActiveSkillSpawnManager;
class UElectricEffectPool;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill");
	USkillManager* SkillManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill");
	UTimerSkillSpawnManagerComponent* TimerSkillSpawnManagerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill");
	UActiveSkillSpawnManager* ActiveSkillSpawnManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill");
	UPassiveSkillManager* PassiveSkillManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UElectricEffectPool> ElectricEffectPool;

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �̵�
	void Move(const FInputActionValue& value);

	// ���콺 ȸ��
	virtual void Look(const FInputActionValue& value);

	// ����
	virtual void StartJump(const FInputActionValue& value);
	virtual void StopJump(const FInputActionValue& value);
	
	// �޸���
	void StartSprint(const FInputActionValue& value);
	void StopSprint(const FInputActionValue& value);

	// ����
	virtual void Attack(const FInputActionValue& value);

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
	virtual void GetUpgradeStatus();

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
	int32 HP;

	// �ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxHP;

	// ���� ���¹̳�
	int32 Stamina;

	// �ִ� ���¹̳�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxStamina;

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

	// ���¹̳� ȸ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 StaminaRecoveryAmount;

	// ���¹̳� �Ҹ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 StaminaConsumAmount;

	// ���¹̳� ȸ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float StaminaRecoveryAndConsumDelay;

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 Defense;

	// ���� ����� (����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float AttackDelay;

	// ġ��Ÿ Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CriticalProb;

	// ġ��Ÿ Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float CriticalMultiplier;

	// ȸ�� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 EvasionProb;

	// ���� ����ġ
	int32 CurrentExp;

	// �ִ� ����ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxExp;

	// ���� ����
	int32 CurrentLevel;

	// �ִ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxLevel;

	// ����ġ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Shop")
	float ExpMultipler;

	// ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Shop")
	float GoldMultipler;

	// ������ �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 ItemDropProb;

	// ��Ȱ Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Shop")
	int32 RevivalCount;

	// �޸��� ��
	bool IsSprint;

	// �ɱ� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Anim")
	bool IsCrouch;

	// �ǰ� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim")
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

	// ���� �� ���� ĳ�� - Fey
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Crouch")
	bool CanCrouchCharacter;

private:
	// ĸ�� ���� <- �ɱ⿡�� ���
	float CapsuleHeight;

	// ���¹̳�
	FTimerHandle StaminaDelayHandle;

	void UpdateStamina();

protected:
	bool IsDie;

	AEdmundGameState* CurrentGameState;

	virtual bool CheckAction();
};
