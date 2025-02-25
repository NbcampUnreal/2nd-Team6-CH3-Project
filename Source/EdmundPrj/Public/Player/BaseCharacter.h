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

	// 이동
	void Move(const FInputActionValue& value);
	void StopMove(const FInputActionValue& value);

	// 마우스 회전
	void Look(const FInputActionValue& value);

	// 점프
	void StartJump(const FInputActionValue& value);
	void StopJump(const FInputActionValue& value);
	
	// 달리기
	void StartSprint(const FInputActionValue& value);
	void StopSprint(const FInputActionValue& value);

	// 공격
	void Attack(const FInputActionValue& value);
	virtual bool ActiveWeapon();

	// 근접공격
	void MeleeAttack(const FInputActionValue& value);
	void EndMeleeAttack();

	// 재장전
	void ReloadAction(const FInputActionValue& value);
	virtual void Reload();

	// 상호작용
	void Interaction(const FInputActionValue& value);

	// 줌 인/아웃
	void ZoomIn(const FInputActionValue& value);
	void ZoomOut(const FInputActionValue& value);

	// 앉기
	void StartCrouch(const FInputActionValue& value);
	void StopCrouch(const FInputActionValue& value);

	// 피격
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	// 경험치 증가
	void AddExp(int32 Exp);

	// 레벨업
	void LevelUp();

	// 현재 체력 Getter
	float GetHP() const;

	// 현재 체력 Setter
	void SetHP(float NewHP);

	// 체력 회복
	void AmountHP(float AmountHP);

	// 탄환 Setter
	void SetAmmo(int32 NewAmmo);

	// 탄환 더하기
	void AmountAmmo(int32 AmountAmmo);

	// 현재 골드 Getter
	int32 GetGold() const;

	// 골드 획득
	void AddGold(int32 Gold);

	// 강화해놓은 스테이터스값 받기
	void GetUpgradeStatus();

	// 죽음
	void ActiveDieAction();

public:
	// 캐릭터 타입 반환
	ECharacterType getCharacterType();

	void MeleeAttackTrace();

	// 공격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* AttackMontage;

	// 캐릭터 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	ECharacterType CharacterType;

	// 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float HP;

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float MaxHP;

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

	// 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 Defense;

	// 공격 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float AttackSpeed;

	// 최대 탄환
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxAmmo;

	// 현재 탄환
	int32 CurrentAmmo;

	// 보유 스킬
	/*FVector<ActiveSkill> MyActiveSkill;
	FVector<PassiveSkill> MyPassiveSkill;*/

	// 치명타 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CriticalProb;

	// 회피 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 EvasionProb;

	// 현재 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CurrentExp;

	// 최대 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxExp;

	// 현재 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CurrentLevel;

	// 최대 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxLevel;

	// 현재 골드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CurrentGold;

	// 부활 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 RevivalCount;

	// 달리는 중
	bool IsSprint;

	// 앉기 중
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	bool IsCrouch;

	// 걷기 중
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	bool IsMove;

	// 재장전 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	UAnimMontage* MeleeAttackMontage;

	// 재장전 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float MeleeAttackDelay;

	// 피격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	UAnimMontage* HitActionMontage;

	// 죽음 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	UAnimMontage* DieActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> MeleeAttackSound;	// 근접공격 사운드

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> ReloadSound;			// 재장전 사운드

private:
	// 캡슐 높이 <- 앉기에서 사용
	float CapsuleHeight;

	FTimerHandle MeleeAttackDelayHandle;

protected:
	bool bIsMeleeAttack;
	bool bIsAttack;
	bool bIsReloading;
	bool IsDie;

	bool CheckAction();
};
