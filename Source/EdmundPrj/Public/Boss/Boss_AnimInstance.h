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
    void AnimNotify_AttackCheck();

    void InitializeBossRef();


    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<ABoss> BossRef;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<class UCharacterMovementComponent> MovementComponent;

    UPROPERTY(BlueprintReadOnly)
    float MonsterMoveSpeed;

    UPROPERTY(BlueprintReadOnly)
    int32 Attack1Count;

    UPROPERTY(BlueprintReadOnly)
    bool bIsFalling;

    UPROPERTY(BlueprintReadOnly)
    bool bIsMoving;

    UPROPERTY(BlueprintReadOnly)
    float Fly;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill2")
    bool bIsSkill2 = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 Skill2HintType = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill2")
    int32 Skill2Iteration = 0;
    
    UPROPERTY(BlueprintReadOnly)
    int32 UpDown = 1;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Initialize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Attack1Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Attack2Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Attack3Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Attack4Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Skill1Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Skill2Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Skill3Montage;


    UFUNCTION(BlueprintCallable)
    bool GetbIsFalling() const { return bIsFalling; }

    void SetSkill2Iteration(int32 NewSkill2Iteration) { Skill2Iteration = NewSkill2Iteration; }
    void SetSkill2HintType(int32 NewSkill2HintType) { Skill2Iteration = NewSkill2HintType; }
    void SetbIsSkill2(bool NewbIsSkill2) { bIsSkill2 = NewbIsSkill2; }
};