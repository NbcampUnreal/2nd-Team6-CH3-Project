// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/AnimNotify_MeleeAttackCheck.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


void UAnimNotify_MeleeAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

    if (MeshComp && MeshComp->GetOwner())
    {
        AActor* Owner = MeshComp->GetOwner();
        UCapsuleComponent* CollisionComp = NewObject<UCapsuleComponent>(Owner);
        CollisionComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale);

        // 콜리전 컴포넌트 초기화
        CollisionComp->SetCapsuleSize(50.0f, 100.0f); // 필요에 따라 사이즈 조정
        CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
        CollisionComp->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f)); // 액터의 정중앙에 위치시키기 위해 설정

        CollisionComp->RegisterComponent();

        CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UAnimNotify_MeleeAttackCheck::OnOverlapBegin);

        CollisionComp->DestroyComponent();
    }
}

void UAnimNotify_MeleeAttackCheck::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
    {
        AActor* Owner = OverlappedComp->GetOwner();  // OverlappedComp는 CollisionComp를 의미
        ABaseMonster* Monster = Cast<ABaseMonster>(Owner);
        if (Monster)
        {
            float DamageValue = Monster->GetMonsterAttackDamage();

            UGameplayStatics::ApplyDamage(
                OtherActor,
                DamageValue,
                nullptr,
                nullptr,
                UDamageType::StaticClass()
            );

            OverlappedComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}


//void UAnimNotify_MeleeAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
//{
//    if (MeshComp && MeshComp->GetOwner())
//    {
//        AActor* Owner = MeshComp->GetOwner();
//        UCapsuleComponent* CollisionComp = NewObject<UCapsuleComponent>(Owner);
//        CollisionComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
//
//        // 콜리전 컴포넌트 초기화
//        CollisionComp->SetCapsuleSize(50.0f, 100.0f); // 필요에 따라 사이즈 조정
//        CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
//
//        CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &UAnimNotify_MeleeAttackCheck::OnOverlapBegin);
//
//        // 콜리전 컴포넌트 활성화
//        CollisionComp->RegisterComponent();
//    }
//}
//
//void UAnimNotify_MeleeAttackCheck::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//    if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
//    {
//        AActor* Owner = OverlappedComp->GetOwner();
//        ABaseMonster* Monster = Cast<ABaseMonster>(Owner);
//        if (Monster)
//        {
//            float DamageValue = Monster->GetMonsterAttackDamage();
//            UGameplayStatics::ApplyDamage(OtherActor, DamageValue, nullptr, OverlappedComp->GetOwner(), DamageTypeClass);
//        }
//    }
//}


