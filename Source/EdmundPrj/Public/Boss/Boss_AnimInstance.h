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

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill1")
    bool bIsSkill1 = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill1")
    int32 Skill1HintType = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill1")
    int32 Skill1Iteration = 0;
    
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
    UAnimMontage* Skill1_1Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Skill1_2Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Skill1_3Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Skill1_4Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Skill2Montage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* Skill3Montage;


    UFUNCTION(BlueprintCallable)
    bool GetbIsFalling() const { return bIsFalling; }

    void SetSkill1Iteration(int32 NewSkill2Iteration) { Skill1Iteration = NewSkill2Iteration; }
    void SetSkill1HintType(int32 NewSkill2HintType) { Skill1HintType = NewSkill2HintType; }
    void SetbIsSkill1(bool NewbIsSkill2) { bIsSkill1 = NewbIsSkill2; }
};