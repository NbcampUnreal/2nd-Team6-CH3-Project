// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Monster/BaseMonster.h"
#include "AnimNotify_MeleeAttackCheck.generated.h"

UCLASS()
class EDMUNDPRJ_API UAnimNotify_MeleeAttackCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

//public:
//    // Notify 함수 오버라이드
//    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
//
//protected:
//    // 콜리전 이벤트 함수
//    UFUNCTION()
//    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//
//    // 데미지 양
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
//    float DamageAmount;
//
//    // 데미지 타입
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
//    TSubclassOf<UDamageType> DamageTypeClass;