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

	// 공격 보이스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TArray<TObjectPtr<USoundBase>> AttackSounds;

	// 공격 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TArray<TObjectPtr<USoundBase>> WeaponSounds;

	// Notify에서 호출하기 위해 public
	void AttackTrace();

	void NextCombo();

	// 콤보 초기화 딜레이
	float ResetDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	// 공격 크기
	float AttackRangeRadius;

protected:
	// 공격
	virtual void Attack(const FInputActionValue& value) override;

private:
	int32 ComboCount;
	float ComboTimeDuration;

	FTimerHandle ComboResetTimerHandle;

	void ResetCombo();
};
