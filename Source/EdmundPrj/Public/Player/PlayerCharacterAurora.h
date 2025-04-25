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

	// Notify에서 호출
	void AttackTrace();
	void NextCombo();

protected:
	// 공격
	virtual void Attack(const FInputActionValue& value) override;

private:
	// 공격 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float AttackRangeRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float AttackForwardOffset;

	// 공격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> AttackMontages = { };

	// 콤보 초기화 딜레이
	float ResetDelay;

	int32 ComboCount;

	float ComboTimeDuration;

	FTimerHandle ComboResetTimerHandle;

	void ResetCombo();
};
