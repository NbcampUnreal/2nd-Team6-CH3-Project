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

	// 공격력 Getter
	float GetAttackDamage() const;

	// 공격력 Setter
	void SetAttackDamage(float NewAttackDamage);

	// 공격력 Getter
	float GetAttackDelay() const;

	// 경험치 증가
	void AddExp(int32 Exp);

	// 레벨업
	void LevelUp();

	// 현재 체력 Getter
	int32 GetHP() const;

	// 현재 체력 Setter
	void SetHP(int32 NewHP);

	// 체력 회복
	void AmountHP(int32 AmountHP);

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

	// 상호작용
	void PauseAction(const FInputActionValue& value);

	// 피격
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	// 강화해놓은 스테이터스값 받기
	void GetUpgradeStatus();

	// 죽음
	virtual void ActiveDieAction();

public:
	// 캐릭터 타입 반환
	ECharacterType GetCharacterType();

	virtual void AttackTrace();

	// 캐릭터 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	ECharacterType CharacterType;

	// 현재 체력
	int32 HP;

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxHP;

	// 현재 스태미나
	int32 Stamina;

	// 최대 스태미나
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxStamina;

	// 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float WalkSpeed;

	// 달리기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float SprintSpeed;

	// 앉아서 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float CrouchMoveSpeed;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float AttackDamage;

	// 스태미나 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 StaminaRecoveryAmount;

	// 스태미나 소모량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 StaminaConsumAmount;

	// 스태미나 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float StaminaRecoveryAndConsumDelay;

	// 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 Defense;

	// 공격 딜레이 (공속)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float AttackDelay;

	// 치명타 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CriticalProb;

	// 치명타 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float CriticalMultiplier;

	// 회피 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 EvasionProb;

	// 현재 경험치
	int32 CurrentExp;

	// 최대 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxExp;

	// 현재 레벨
	int32 CurrentLevel;

	// 최대 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxLevel;

	// 경험치 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Shop")
	float ExpMultipler;

	// 골드 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Shop")
	float GoldMultipler;

	// 아이템 드랍률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 ItemDropProb;

	// 부활 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Shop")
	int32 RevivalCount;

	// 달리는 중
	bool IsSprint;

	// 앉기 중
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Anim")
	bool IsCrouch;

	// 피격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Anim")
	UAnimMontage* HitActionMontage;

	// 죽음 애니메이션
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

	// 앉을 수 없는 캐릭 - Fey
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Crouch")
	bool CanCrouchCharacter;

private:
	// 캡슐 높이 <- 앉기에서 사용
	float CapsuleHeight;

	// 스태미나
	FTimerHandle StaminaDelayHandle;

	void UpdateStamina();

protected:
	bool IsDie;

	AEdmundGameState* CurrentGameState;

	virtual bool CheckAction();
};
