// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/NPCMonster.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NPC/NPCAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

// 몬스터 스탯 설정은 여기서!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 여긴 NPC라 적용안됨
void ANPCMonster::SetMonsterStatsByLevel()
{
    MonsterHP = 100 + (MonsterLevel * 50);
    MonsterMaxHP = 100 + (MonsterLevel * 50);
    MonsterAttackDamage = 10.0f + (MonsterLevel * 5.0f);
    MonsterArmor = 5.0f + (MonsterLevel * 2.0f);
}


ANPCMonster::ANPCMonster()
{
    NpcType = ENpcType::Baldor;
    //MonsterType = EMonsterType::Melee;
}

void ANPCMonster::BeginPlay()
{
	Super::BeginPlay();

    this->Tags.Empty();
    this->Tags.Add(FName("NPC"));

    //SetBondageMode(true);
    //SetMoveMode(true);
    SetBattleMode(true);

    MonsterMoveSpeed = 500.0f;
    MonsterChaseSpeed = 500.0f;

    UpdatePatrolSpeed();

    GameState = Cast<AEdmundGameState>(UGameplayStatics::GetGameState(GetWorld()));

    MonsterHP = 100 + (MonsterLevel * 50);
    MonsterMaxHP = 100 + (MonsterLevel * 50);
    MonsterAttackDamage = 10.0f + (MonsterLevel * 5.0f);
    MonsterArmor = 5.0f + (MonsterLevel * 2.0f);
}



void ANPCMonster::MonsterAttackCheck()
{

    USkeletalMeshComponent* MeshComp = GetMesh();
    ABaseMonster* Monster = Cast<ABaseMonster>(this);

    if (Monster)
    {

        UCapsuleComponent* CollisionComp = NewObject<UCapsuleComponent>(this);

        if (IsValid(CollisionComp))
        {
            CollisionComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale);

            // 콜리전 컴포넌트 초기화
            CollisionComp->SetCapsuleSize(300.0f, 300.0f); // 필요에 따라 사이즈 조정
            CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
            CollisionComp->SetRelativeLocation(FVector(0.0f, 0, 0.0f)); // 액터의 앞에 콜리전 위치

            CollisionComp->RegisterComponent();

            CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ANPCMonster::OnOverlapBegin);
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
void ANPCMonster::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(FName("Monster")))
    {

        UParticleSystemComponent* Particle = nullptr;

        if (AttackParticle)
        {
            FVector ParticleScale = FVector(1.0f, 1.0f, 1.0f);
            FVector ParticleLocation = GetActorLocation();

            Particle = UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                AttackParticle,
                ParticleLocation,
                GetActorRotation(),
                ParticleScale,
                false
            );
        }

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

void ANPCMonster::NPCMonsterAttack()
{
    
    PlaySound();

    UParticleSystemComponent* Particle = nullptr;

    if (SpawnParticle)
    {
        FVector ParticleScale = FVector(1.0f, 1.0f, 1.0f);
        FVector ParticleLocation = GetActorLocation();

        Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            SpawnParticle,
            ParticleLocation,
            GetActorRotation(),
            ParticleScale,
            false
        );
    }

    Super::MonsterAttack();
}

void ANPCMonster::SetBattleMode(bool NewState)
{

    SetMoveMode(false);

    //SetChaseMode(!NewState);

    GetWorld()->GetTimerManager().ClearTimer(BondageTimerHandle);

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

    bIsBondageMode = NewState;

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    AnimInstance->Montage_Play(BondageAnimation);

    float AnimDuration = BondageAnimation->GetPlayLength();

    GetWorld()->GetTimerManager().SetTimer(BondageTimerHandle, this, &ANPCMonster::PlayBondageMontage, AnimDuration - 1.0f, true);
}

void ANPCMonster::SetMoveMode(bool NewState)
{
        AAIController* AIController = Cast<AAIController>(GetController());
        if (AIController)
        {
            AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsModeMode"), NewState);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("NPC BattleMode 실행중: AIController가 없습니다."));
        }
}

void ANPCMonster::InitSpawnParticlePlay()
{
    UParticleSystemComponent* InitParticle = nullptr;

    if (InitSpawnParticle)
    {
        FVector ParticleScale = FVector(1.0f, 1.0f, 1.0f);
        FVector ParticleLocation = GetActorLocation();

        InitParticle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            InitSpawnParticle,
            ParticleLocation,
            GetActorRotation(),
            ParticleScale,
            false
        );
    }
}

void ANPCMonster::PlayBondageMontage()
{
    UAnimInstance* LoopAnimInstance = GetMesh()->GetAnimInstance();

    LoopAnimInstance->Montage_Play(BondageAnimation);
}

void ANPCMonster::PlaySound()
{
    //CurrentAudioComp->SetSound(AttackSound);
    //CurrentAudioComp->Play();

    if (GameState)
    {
        GameState->PlayNpcSound(CurrentAudioComp, NpcType, ESoundType::Attack);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("NPC게임스테이트없음"));
    }
    //GameState->PlayMonsterSound(CurrentAudioComp, MonsterType, ESoundType::Avoid);
}
