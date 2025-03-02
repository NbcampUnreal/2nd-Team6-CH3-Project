#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BossAttack3.generated.h"

UCLASS()
class EDMUNDPRJ_API UAnimNotify_BossAttack3 : public UAnimNotify
{
    GENERATED_BODY()

public:
    // 노티파이 종류를 문자열로 설정 (예: "BossAttack3_1", "BossAttack3_2", "BossAttack3_3")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossAttack")
    FName NotifyName;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
