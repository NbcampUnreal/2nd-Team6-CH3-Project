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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Sound")
	TObjectPtr<USoundBase> AttackSound;

	// Notify���� ȣ���ϱ� ���� public
	void AttackTrace();

	void NextCombo();

	// �޺� �ʱ�ȭ ������
	float ResetDelay;

	// ���� ũ��
	float AttackRangeRadius;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ����
	void Attack(const FInputActionValue& value);

private:
	bool IsAttack;
	int32 ComboCount;

	FTimerHandle ComboResetTimerHandle;

	void ResetCombo();
};
