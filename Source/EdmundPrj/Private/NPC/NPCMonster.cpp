// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/NPCMonster.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NPC/NPCAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

ANPCMonster::ANPCMonster()
{
}

void ANPCMonster::BeginPlay()
{
	Super::BeginPlay();

    this->Tags.Empty();
    this->Tags.Add(FName("NPC"));

    //SetBondageMode(true);
    SetBattleMode(true);
}



void ANPCMonster::MonsterAttackCheck()
{

    USkeletalMeshComponent* MeshComp = GetMesh();
    ABaseMonster* Monster = Cast<ABaseMonster>(this);

    if (Monster)
    {
        PlaySound();

        UCapsuleComponent* CollisionComp = NewObject<UCapsuleComponent>(this);
        CollisionComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale);

        // 콜리전 컴포넌트 초기화
        CollisionComp->SetCapsuleSize(100.0f, 100.0f); // 필요에 따라 사이즈 조정
        CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
        CollisionComp->SetRelativeLocation(FVector(0.0f, 150.0f, 0.0f)); // 액터의 앞에 콜리전 위치

        CollisionComp->RegisterComponent();

        CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ANPCMonster::OnOverlapBegin);


        //  공격 Collision Visible 활성화
         FVector CapsuleLocation = CollisionComp->GetComponentLocation();
         DrawDebugCapsule(GetWorld(), CapsuleLocation, CollisionComp->GetScaledCapsuleHalfHeight(), CollisionComp->GetScaledCapsuleRadius(), FQuat::Identity, FColor::Green, true, 1.0f);


         // 타이머 X시, 이벤트가 끝나기 전 Destory됨. 왜일까,,
        FTimerHandle TimerHandle;
        this->GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=]()
            {
                if (CollisionComp)
                {
                    CollisionComp->DestroyComponent();
                }
            }), 0.01f, false);
    }
}
void ANPCMonster::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(FName("Monster")))
    {

        UE_LOG(LogTemp, Warning, TEXT("Player Attack Succeed")); // 공격 성공 Log
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

void ANPCMonster::SetBattleMode(bool NewState)
{
    SetChaseMode(!NewState);
    bIsFightMode = NewState;

    if (NewState)
    {
        AAIController* AIController = Cast<AAIController>(GetController());
        if (AIController)
        {
            AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsBattleMode"), NewState);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("NPC BattleMode 실행중: AIController가 없습니다."));
        }
    }

}
void ANPCMonster::SetBondageMode(bool NewState)
{
    SetChaseMode(!NewState);
    bIsBondageMode = NewState;
}

//void ANPCMonster::PlaySound()
//{
//    CurrentAudioComp->SetSound(AttackSound);
//    CurrentAudioComp->Play();
//}
