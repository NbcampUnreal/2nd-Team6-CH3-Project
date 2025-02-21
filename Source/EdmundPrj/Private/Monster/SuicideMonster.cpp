// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/SuicideMonster.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

void ASuicideMonster::MonsterAttackCheck()
{
    USkeletalMeshComponent* MeshComp = GetMesh();
    ABaseMonster* Monster = Cast<ABaseMonster>(this);

    if (Monster)
    {
        UCapsuleComponent* CollisionComp = NewObject<UCapsuleComponent>(this);
        CollisionComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale);

        // 콜리전 컴포넌트 초기화
        CollisionComp->SetCapsuleSize(3000.0f, 3000.0f); // 필요에 따라 사이즈 조정
        CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
        CollisionComp->SetRelativeLocation(FVector(0.0f, 10.0f, 0.0f)); // 액터의 앞에 콜리전 위치

        CollisionComp->RegisterComponent();

        CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASuicideMonster::OnOverlapBegin);


        //  공격 Collision Visible 활성화
         FVector CapsuleLocation = CollisionComp->GetComponentLocation();
         DrawDebugCapsule(GetWorld(), CapsuleLocation, CollisionComp->GetScaledCapsuleHalfHeight(), CollisionComp->GetScaledCapsuleRadius(), FQuat::Identity, FColor::Green, true, 1.0f);


         // 타이머 X시, 이벤트가 끝나기 전 Destory됨. 왜일까,,
        FTimerHandle TimerHandle;
        this->GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=]()
            {
                CollisionComp->DestroyComponent();
            }), 0.01f, false);

        FTimerHandle DeadTimerHandle;
        this->GetWorldTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([=]()
            {
                Monster->MonsterDead();
            }), 0.01f, false);
    }
}
void ASuicideMonster::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
    {
        UE_LOG(LogTemp, Warning, TEXT("Suicide Attack Succeed")); // 공격 성공 Log
        AActor* LocalOwner = OverlappedComp->GetOwner();  // OverlappedComp는 CollisionComp를 의미
        ABaseMonster* Monster = Cast<ABaseMonster>(LocalOwner);
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
        }
    }
}