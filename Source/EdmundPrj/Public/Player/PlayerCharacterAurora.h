#pragma once

#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
#include "PlayerCharacterAurora.generated.h"

struct FInputActionValue;

UCLASS()
class EDMUNDPRJ_API APlayerCharacterAurora : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacterAurora();

	virtual void BeginPlay() override;

	// 공격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> AttackSound;

	// Notify에서 호출하기 위해 public
	void AttackTrace();

	void NextCombo();

	// 콤보 초기화 딜레이
	float ResetDelay;

	// 공격 크기
	float AttackRangeRadius;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 공격
	void Attack(const FInputActionValue& value);

private:
	bool IsAttack;
	int32 ComboCount;

	FTimerHandle ComboResetTimerHandle;

	void ResetCombo();
};
