#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "System/EnumSet.h"
#include "Components/AudioComponent.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBerserkerSkillActivate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBloodAbsorbingSkillActivate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnElectricChainSkillActivate, FVector, MonsterLocation);

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class AEdmundGameState;
class USkillManager;
class UTimerSkillSpawnManagerComponent;
class UPassiveSkillManager;
class UActiveSkillSpawnManager;
class UElectricEffectPool;
class ASupportCharacter;

UCLASS()
class EDMUNDPRJ_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	
	// ���ݷ� Getter
	virtual float GetAttackDamage() const;

	// ���ݷ� Getter
	virtual float GetDamage() const;

	// ���ݷ� Setter
	void SetAttackDamage(const float NewAttackDamage);

	// ���ݷ� Getter
	float GetAttackDelay() const;

	// Ű Getter
	float GetHeight() const;

	// ����ġ ����
	void AddExp(const int32 Exp);

	// ������
	void LevelUp();

	// �ִ� ü�� Getter
	int32 GetMaxHP() const;

	// �ִ� ü�� Setter
	void SetMaxHP(const int32 NewMaxHP);

	// ���� ü�� Getter
	int32 GetHP() const;

	// ���� ü�� Setter
	void SetHP(const int32 NewHP);

	// ü�� ȸ��
	void AmountHP(const int32 AmountHP);

	// �ִ� ���¹̳� Getter
	int32 GetMaxStamina() const;

	// �ִ� ���¹̳� Setter
	void SetMaxStamina(const int32 NewMaxStamina);

	// ���� ���¹̳� Getter
	int32 GetCurrentStamina() const;

	// ���¹̳� ȸ���� Getter
	int32 GetStaminaRecoveryAmount() const;

	// ���¹̳� ȸ���� Setter
	void SetStaminaRecoveryAmount(int32 NewStaminaRecoveryAmount);

	// ����ġ ���� Getter
	float GetExpMultipler() const;

	// ��� ���� Getter
	float GetGoldMultipler() const;

	// ������ ����� Getter
	int32 GetItempDropProb() const;

	// ĳ���� Ÿ�� ��ȯ
	ECharacterType GetCharacterType() const;

	virtual void AttackTrace();

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnBerserkerSkillActivate OnBerserkerSkillActivate;

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnBloodAbsorbingSkillActivate OnBloodAbsorbingSkillActivate;

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnElectricChainSkillActivate OnElectricChainSkillActivate;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<ASupportCharacter> SupportCharClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<ASupportCharacter> SupportCharInstance;

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

	// ����
	void PauseAction(const FInputActionValue& value);

	// �̼�â
	void MissionOnAction(const FInputActionValue& value);

	// �ǰ�
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	// ��ȭ�س��� �������ͽ��� �ޱ�
	virtual void GetUpgradeStatus();

	virtual void ActiveDieAction();

	virtual bool CheckAction();

	AEdmundGameState* CurrentGameState;

	// �ǰ� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> HitActionMontage = nullptr;

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TObjectPtr<UAnimMontage> DieActionMontage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UAudioComponent> CurrentAudioComp = nullptr;

	// �ִ� ���¹̳�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxStamina;

	// ���ݷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float AttackDamage;

	// ���� ����� (����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float AttackDelay;

	// ġ��Ÿ Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CriticalProb;

	// ġ��Ÿ Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float CriticalMultiplier;

	// ���� �� ���� ĳ�� - Fey
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Crouch")
	bool CanCrouchCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Crouch")
	bool IsCrouch;

	// ���� ���¹̳�
	int32 Stamina;

	bool IsSprint;
	bool IsDie;
	bool IsAttack;

private:
	void UpdateStamina();

	// ĳ���� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	ECharacterType CharacterType;

	// �ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 MaxHP;

	// ���� ü��
	int32 HP;

	// ĳ���� Ű
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float Height;

	// �ȱ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;

	// �޸��� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;

	// �ɾƼ� �̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float CrouchMoveSpeed;

	// ���� ĸ�� ũ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float CrouchHeightMultiplier;

	// ���¹̳� ȸ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 StaminaRecoveryAmount;

	// ���¹̳� �Ҹ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 StaminaConsumAmount;

	// ���¹̳� ȸ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryAndConsumDelay;

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 Defense;

	// ȸ�� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 EvasionProb;

	// ���� ����ġ
	int32 CurrentExp;

	// �ִ� ����ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 MaxExp;

	// ���� ����
	int32 CurrentLevel;

	// �ִ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 MaxLevel;

	// ������ �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 ItemDropProb;

	// ����ġ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Shop", meta = (AllowPrivateAccess = "true"))
	float ExpMultipler;

	// ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Shop", meta = (AllowPrivateAccess = "true"))
	float GoldMultipler;

	// ��Ȱ Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Shop", meta = (AllowPrivateAccess = "true"))
	int32 RevivalCount;

	// ĸ�� ���� <- �ɱ⿡�� ���
	float CapsuleHeight;

	// ���¹̳�
	FTimerHandle StaminaDelayHandle;
};
