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

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TArray<UAnimMontage*> AttackMontages;

	// Notify���� ȣ���ϱ� ���� public
	void AttackTrace();

	void NextCombo();

	// �޺� �ʱ�ȭ ������
	float ResetDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	// ���� ũ��
	float AttackRangeRadius;

protected:
	// ����
	virtual void Attack(const FInputActionValue& value) override;

private:
	int32 ComboCount;
	float ComboTimeDuration;

	FTimerHandle ComboResetTimerHandle;

	void ResetCombo();
};
