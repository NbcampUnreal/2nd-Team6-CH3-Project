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
	
	// 공격력 Getter
	virtual float GetAttackDamage() const;

	// 공격력 Getter
	virtual float GetDamage() const;

	// 공격력 Setter
	void SetAttackDamage(const float NewAttackDamage);

	// 공격력 Getter
	float GetAttackDelay() const;

	// 키 Getter
	float GetHeight() const;

	// 경험치 증가
	void AddExp(const int32 Exp);

	// 레벨업
	void LevelUp();

	// 최대 체력 Getter
	int32 GetMaxHP() const;

	// 최대 체력 Setter
	void SetMaxHP(const int32 NewMaxHP);

	// 현재 체력 Getter
	int32 GetHP() const;

	// 현재 체력 Setter
	void SetHP(const int32 NewHP);

	// 체력 회복
	void AmountHP(const int32 AmountHP);

	// 최대 스태미나 Getter
	int32 GetMaxStamina() const;

	// 최대 스태미나 Setter
	void SetMaxStamina(const int32 NewMaxStamina);

	// 현재 스태미나 Getter
	int32 GetCurrentStamina() const;

	// 스태미나 회복량 Getter
	int32 GetStaminaRecoveryAmount() const;

	// 스태미나 회복량 Setter
	void SetStaminaRecoveryAmount(int32 NewStaminaRecoveryAmount);

	// 경험치 배율 Getter
	float GetExpMultipler() const;

	// 골드 배율 Getter
	float GetGoldMultipler() const;

	// 아이템 드랍률 Getter
	int32 GetItempDropProb() const;

	// 캐릭터 타입 반환
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

	// 이동
	void Move(const FInputActionValue& value);

	// 마우스 회전
	virtual void Look(const FInputActionValue& value);

	// 점프
	virtual void StartJump(const FInputActionValue& value);
	virtual void StopJump(const FInputActionValue& value);
	
	// 달리기
	void StartSprint(const FInputActionValue& value);
	void StopSprint(const FInputActionValue& value);

	// 공격
	virtual void Attack(const FInputActionValue& value);

	// 상호작용
	void Interaction(const FInputActionValue& value);

	// 앉기
	void StartCrouch(const FInputActionValue& value);
	void StopCrouch(const FInputActionValue& value);

	// 퍼즈
	void PauseAction(const FInputActionValue& value);

	// 미션창
	void MissionOnAction(const FInputActionValue& value);

	// 피격
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	// 강화해놓은 스테이터스값 받기
	virtual void GetUpgradeStatus();

	virtual void ActiveDieAction();

	virtual bool CheckAction();

	AEdmundGameState* CurrentGameState;

	// 피격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> HitActionMontage = nullptr;

	// 죽음 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TObjectPtr<UAnimMontage> DieActionMontage = nullptr;

	TObjectPtr<UAudioComponent> CurrentAudioComp = nullptr;

	// 최대 스태미나
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxStamina;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float AttackDamage;

	// 공격 딜레이 (공속)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float AttackDelay;

	// 치명타 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CriticalProb;

	// 치명타 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float CriticalMultiplier;

	// 앉을 수 없는 캐릭 - Fey
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Crouch")
	bool CanCrouchCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Crouch")
	bool IsCrouch;

	// 현재 스태미나
	int32 Stamina;

	bool IsSprint;
	bool IsDie;
	bool IsAttack;

private:
	void UpdateStamina();

	// 캐릭터 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	ECharacterType CharacterType;

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 MaxHP;

	// 현재 체력
	int32 HP;

	// 캐릭터 키
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float Height;

	// 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;

	// 달리기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;

	// 앉아서 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float CrouchMoveSpeed;

	// 스태미나 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 StaminaRecoveryAmount;

	// 스태미나 소모량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 StaminaConsumAmount;

	// 스태미나 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryAndConsumDelay;

	// 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 Defense;

	// 회피 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 EvasionProb;

	// 현재 경험치
	int32 CurrentExp;

	// 최대 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 MaxExp;

	// 현재 레벨
	int32 CurrentLevel;

	// 최대 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 MaxLevel;

	// 아이템 드랍률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	int32 ItemDropProb;

	// 경험치 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Shop", meta = (AllowPrivateAccess = "true"))
	float ExpMultipler;

	// 골드 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Shop", meta = (AllowPrivateAccess = "true"))
	float GoldMultipler;

	// 부활 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Shop", meta = (AllowPrivateAccess = "true"))
	int32 RevivalCount;

	// 캡슐 높이 <- 앉기에서 사용
	float CapsuleHeight;

	// 스태미나
	FTimerHandle StaminaDelayHandle;
};
