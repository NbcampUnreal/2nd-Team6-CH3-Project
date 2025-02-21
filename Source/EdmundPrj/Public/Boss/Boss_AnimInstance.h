#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Boss_AnimInstance.generated.h"

UCLASS()
class EDMUNDPRJ_API UBoss_AnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsMoving;
};
