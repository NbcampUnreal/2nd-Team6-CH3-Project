// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/SuicideMonster.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/Actor.h"

// 몬스터 스탯 설정은 여기서!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void ASuicideMonster::SetMonsterStatsByLevel()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (IsValid(PlayerController))
    {
        APawn* PlayerPawn = PlayerController->GetPawn();
        if (IsValid(PlayerPawn))
        {
            ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(PlayerPawn);
            if (PlayerCharacter)
            {
                MonsterHP = 100 + (MonsterLevel * 50);
                MonsterMaxHP = 100 + (MonsterLevel * 50);
                MonsterAttackDamage = 30.0f + (MonsterLevel * 20.0f);
                MonsterArmor = -100.0f + (MonsterLevel * 0.0f);
                MonsterExpReward += 50 * (PlayerCharacter->GetExpMultipler() - 100) / 100;
                MonsterGoldReward += MonsterGoldReward * (PlayerCharacter->GetGoldMultipler() - 100) / 100;
                MonsterHealKitProbability = PlayerCharacter->GetItempDropProb() / 2;
                MonsterGoldProbability = PlayerCharacter->GetItempDropProb();
            }
        }
    }
}


ASuicideMonster::ASuicideMonster()
{
    MonsterType = EMonsterType::Suicide;
}

void ASuicideMonster::MonsterAttackCheck()
{

    SetCanDropReward(false);

    USkeletalMeshComponent* MeshComp = GetMesh();
    ABaseMonster* Monster = Cast<ABaseMonster>(this);

    if (Monster)
    {
        //파티클 재생
        PlayParticle();
        PlaySound();

        UCapsuleComponent* CollisionComp = NewObject<UCapsuleComponent>(this);

        if (IsValid(CollisionComp))
        {
            CollisionComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale);

            // 콜리전 컴포넌트 초기화
            CollisionComp->SetCapsuleSize(2000.0f, 2000.0f); // 필요에 따라 사이즈 조정
            CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
            CollisionComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); // 액터의 앞에 콜리전 위치

            CollisionComp->RegisterComponent();

            CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASuicideMonster::OnOverlapBegin);
        }

         // 타이머 X시, 이벤트가 끝나기 전 Destory됨. 왜일까,,
        FTimerHandle TimerHandle;

        if (IsValid(GetWorld()))
        {
            this->GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=]()
                {
                    if (CollisionComp && CollisionComp->IsValidLowLevel())
                    {
                        CollisionComp->DestroyComponent();
                    }
                }), 0.01f, false);

            FTimerHandle DeadTimerHandle;
            this->GetWorldTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([=]()
                {
                    if (Monster && Monster->IsValidLowLevel())
                    {
                        Monster->MonsterDead();
                    }
                }), 0.01f, false);
        }
    }
}

void ASuicideMonster::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
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

        if (OtherActor->ActorHasTag("NPC"))
        {
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
}


void ASuicideMonster::PlayParticle()
{
    //파티클 활성화, 헤더는 BaseMonster에 있음
    UParticleSystemComponent* Particle = nullptr;

    if (AttackParticle)
    {
        FVector ParticleScale = FVector(0.7f, 0.7f, 0.7f);

        Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            AttackParticle,
            GetActorLocation(),
            GetActorRotation(),
            ParticleScale,
            false
        );
    }
}

void ASuicideMonster::PlaySound()
{
    GameState->PlayMonsterSound(CurrentAudioComp, MonsterType, ESoundType::Attack);
}
