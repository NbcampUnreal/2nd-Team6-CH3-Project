// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MeleeMonster.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/Actor.h"

// 몬스터 스탯 설정은 여기서!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void AMeleeMonster::SetMonsterStatsByLevel()
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
                MonsterAttackDamage = 20.0f + (MonsterLevel * 10.0f);
                MonsterArmor = 10.0f + (MonsterLevel * 10.0f);
                MonsterExpReward += 100 * (PlayerCharacter->GetExpMultipler() - 100) / 100;
                MonsterGoldReward += MonsterGoldReward * (PlayerCharacter->GetGoldMultipler() - 100) / 100;
                MonsterHealKitProbability = PlayerCharacter->GetItempDropProb() / 2;
                MonsterGoldProbability = PlayerCharacter->GetItempDropProb();
            }
        }
    }
}


AMeleeMonster::AMeleeMonster()
{
    MonsterType = EMonsterType::Melee;
}

void AMeleeMonster::MonsterAttackCheck()
{
    USkeletalMeshComponent* MeshComp = GetMesh();
    ABaseMonster* Monster = Cast<ABaseMonster>(this);

    if (Monster)
    {
        PlaySound();

        UCapsuleComponent* CollisionComp = NewObject<UCapsuleComponent>(this);

        if (IsValid(CollisionComp))
        {
            CollisionComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale);

            // 콜리전 컴포넌트 초기화
            CollisionComp->SetCapsuleSize(100.0f, 100.0f); // 필요에 따라 사이즈 조정
            CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
            CollisionComp->SetRelativeLocation(FVector(0.0f, 150.0f, 0.0f)); // 액터의 앞에 콜리전 위치

            CollisionComp->RegisterComponent();

            CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMeleeMonster::OnOverlapBegin);
        }

       //  공격 Collision Visible 활성화
        //FVector CapsuleLocation = CollisionComp->GetComponentLocation();
        //DrawDebugCapsule(GetWorld(), CapsuleLocation, CollisionComp->GetScaledCapsuleHalfHeight(), CollisionComp->GetScaledCapsuleRadius(), FQuat::Identity, FColor::Green, true, 1.0f);


        // 타이머 X시, 이벤트가 끝나기 전 Destory됨. 왜일까,,

        if (GetWorld())
        {
            FTimerHandle TimerHandle;
            this->GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=]()
                {
                    if (CollisionComp && CollisionComp->IsValidLowLevel())
                    {
                        CollisionComp->DestroyComponent();
                    }
                }), 0.01f, false);
        }
    }
}
void AMeleeMonster::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("NPC"))
        {
            PlayParticle();

            //UE_LOG(LogTemp, Warning, TEXT("Player Attack Succeed")); // 공격 성공 Log
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

void AMeleeMonster::PlayParticle()
{
    //파티클 활성화, 헤더는 BaseMonster에 있음
    UParticleSystemComponent* Particle = nullptr;

    if (AttackParticle)
    {
        FVector ParticleScale = FVector(1.0f, 1.0f, 1.0f);
        FVector ParticleLocation = GetActorLocation() + GetActorForwardVector() * 150.0f;

        Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            AttackParticle,
            ParticleLocation,
            GetActorRotation() + FRotator(0.f, 180.f, 0.f),
            ParticleScale,
            false
        );
    }
}

void AMeleeMonster::PlaySound()
{
    GameState->PlayMonsterSound(CurrentAudioComp, MonsterType, ESoundType::Attack);
    //CurrentAudioComp->SetSound(AttackSound);
    //CurrentAudioComp->Play();
}
