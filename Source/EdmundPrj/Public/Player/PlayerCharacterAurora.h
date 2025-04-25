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

	// Notify���� ȣ��
	void AttackTrace();
	void NextCombo();

protected:
	// ����
	virtual void Attack(const FInputActionValue& value) override;

private:
	// ���� ũ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float AttackRangeRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move", meta = (AllowPrivateAccess = "true"))
	float AttackForwardOffset;

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> AttackMontages = { };

	// �޺� �ʱ�ȭ ������
	float ResetDelay;

	int32 ComboCount;

	float ComboTimeDuration;

	FTimerHandle ComboResetTimerHandle;

	void ResetCombo();
};
