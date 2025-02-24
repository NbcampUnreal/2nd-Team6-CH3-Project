#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Boss_AnimInstance.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBoss_AnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    
    UFUNCTION()
    void AnimNotify_Attack1();

    UFUNCTION()
    void AnimNotify_AttackCheck();


    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<ABoss> BossRef;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<class UCharacterMovementComponent> MovementComponent;

    UPROPERTY(BlueprintReadOnly)
    float MonsterMoveSpeed;

    UPROPERTY(BlueprintReadOnly)
    int32 Attack1Count;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Attack1Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Attack2Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Attack3Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Attack4Montage;
};
