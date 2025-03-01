#include "Boss/Boss.h"
#include "Boss/BossState.h"
#include "Boss/BossAIController.h"
#include "Boss/State/Boss_Idle.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ABoss::ABoss()
{
    PrimaryActorTick.bCanEverTick = true;
    AIControllerClass = ABossAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    BossState = nullptr;
    MonsterMoveSpeed = 5000.0f;
    MonsterHP = 500.0f;
    MonsterMaxHP = 1000.0f;
    MonsterAttackDamage = 10.0f;

    // 공격2 범위
    Attack2Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Attack2Collision"));
    Attack2Collision->SetupAttachment(RootComponent);
    Attack2Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Attack2Collision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    Attack2Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    Attack2Collision->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OnAttack2CollisionOverlap);


    // 캡슐
    CapsuleComponent1 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent1"));
    CapsuleComponent1->SetupAttachment(GetMesh(), TEXT("Cap"));
    CapsuleComponent2 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent2"));
    CapsuleComponent2->SetupAttachment(GetMesh(), TEXT("Cap2"));
    CapsuleComponent3 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent3"));
    CapsuleComponent3->SetupAttachment(GetMesh(), TEXT("iuhg"));

    //
    Skill3Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Skill3Particle"));
    Skill3Particle->SetupAttachment(RootComponent);
    Skill3Particle->bAutoActivate = false;

    // 공격 스폰 위치
    MuzzleLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(GetMesh(), TEXT("MuzzleSocket"));

}

void ABoss::BeginPlay()
{
    Super::BeginPlay();
    Tags.Add(TEXT("Boss"));
    if (GetMesh() && GetMesh()->GetAnimInstance())
    {
        AnimInstance = Cast<UBoss_AnimInstance>(GetMesh()->GetAnimInstance());
    }

    InitiallizeBullerPool();
}

void ABoss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (BossState)
    {
        BossState->TickState(DeltaTime);
    }

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = MonsterChaseSpeed;
    }
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            DeltaTime,
            FColor::Green,
            FString::Printf(TEXT("Boss HP: %.1f / %.1f"), MonsterHP, MonsterMaxHP)
        );
    }

    if (CapsuleComponent1 && GetMesh())
    {

        FTransform SocketTransform = GetMesh()->GetSocketTransform(TEXT("Cap"));
        CapsuleComponent1->SetWorldTransform(SocketTransform);
    }

    if (CapsuleComponent2 && GetMesh())
    {
        FTransform SocketTransform = GetMesh()->GetSocketTransform(TEXT("Cap2"));
        CapsuleComponent2->SetWorldTransform(SocketTransform);
    }

    if (CapsuleComponent3 && GetMesh())
    {
        FTransform SocketTransform = GetMesh()->GetSocketTransform(TEXT("Socket3"));
        CapsuleComponent3->SetWorldTransform(SocketTransform);
    }
}

int32 ABoss::SetAttack1Count(int32 NewCount)
{
    Attack1Count = NewCount;
    return Attack1Count;
}

float ABoss::SetMonsterMoveSpeed(float NewSpeed)
{
    MonsterMoveSpeed = NewSpeed;
    return MonsterMoveSpeed;
}

void ABoss::SetState(EBossState NewState)
{
    if (BossState)
    {
        BossState->ExitState();
        BossState = nullptr;
    }

    switch (NewState)
    {
    case EBossState::Idle:
        //BossState = NewObject<UBoss_Idle>(this);
        //if (AnimInstance) AnimInstance->bIsMoving = false;
        break;

    case EBossState::Chase:
        //BossState = NewObject<UBoss_Chase>();
        GetCharacterMovement()->MaxWalkSpeed = GetMonsterMoveSpeed();
        //if (AnimInstance) AnimInstance->bIsMoving = true;
        break;

    case EBossState::Attack1:
        //BossState = NewObject<UBoss_Attack1>();
        break;

    case EBossState::Attack2:
        //BossState = NewObject<UBoss_Attack2>();
        break;

    case EBossState::Attack3:
        //BossState = NewObject<UBoss_Attack3>();
        break;

    case EBossState::Attack4:
        //BossState = NewObject<UBoss_Attack4>();
        break;

    case EBossState::Skill2:
        //BossState = NewObject<UBoss_Skill2>();
        break;

    case EBossState::Skill3:
        //BossState = NewObject<UBoss_Skill3>();
        break;

    default:
        //BossState = nullptr;
        break;
    }

    if (BossState)
    {
        //BossState->EnterState(this);
    }
}

void ABoss::InitiallizeBullerPool()
{
    if (!GetWorld()) return;

    
    for (int32 i = 0; i < PoolSize; ++i)
    {
        ABoss_Attack1_Bullet* Bullet = GetWorld()->SpawnActor<ABoss_Attack1_Bullet>(Attack1BulletClass);
        ABoss_Attack4_Bullet* Bullet4 = GetWorld()->SpawnActor<ABoss_Attack4_Bullet>(Attack4BulletClass);
        ABoss_Skill3_Wall* Wall = GetWorld()->SpawnActor<ABoss_Skill3_Wall>(Skill3WallClass);
        if (Bullet)
        {
            Bullet->SetActorHiddenInGame(true);
            Bullet->SetActorEnableCollision(false);
        }
        if (Bullet4)
        {
            Bullet4->SetActorHiddenInGame(true);
            Bullet4->SetActorEnableCollision(false);
        }
        if (Wall)
        {
            Wall->SetActorHiddenInGame(true);
            Wall->SetActorEnableCollision(false);
        }
    }


}

void ABoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // Attack1 풀 정리
    ABoss_Attack1_Bullet::BulletPool.Empty();
    ABoss_Attack4_Bullet::Bullet4Pool.Empty();
    ABoss_Skill3_Wall::WallPool.Empty();
    
}

void ABoss::UpdateAttackCooldown(int32 AttackID)
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    switch (AttackID)
    {
    case 1:
        Attack1_CooldownEnd = CurrentTime + Attack1_CooldownDuration;
        break;
    case 2:
        Attack2_CooldownEnd = CurrentTime + Attack2_CooldownDuration;
        break;
    case 3:
        Attack3_CooldownEnd = CurrentTime + Attack3_CooldownDuration;
        break;
    case 4:
        Attack4_CooldownEnd = CurrentTime + Attack4_CooldownDuration;
        break;
    default:
        break;
    }
}

void ABoss::MonsterAttackCheck()
{

}

void ABoss::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == nullptr || bInterrupted)
    {
        return;
    }
    GetMesh()->bPauseAnims = true;
}

void ABoss::MonsterDead()
{
    if (bIsDead)
    {
        return;
    }

    SetIsDead(true);

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    }

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->StopMovementImmediately();
        GetCharacterMovement()->DisableMovement();
    }

    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        if (AICon->BrainComponent)
        {
            AICon->BrainComponent->StopLogic(TEXT("Boss is dead"));
        }
        AICon->UnPossess();
    }

    if (USkeletalMeshComponent* MeshComp = GetMesh())
    {
        MeshComp->SetSimulatePhysics(true);
        MeshComp->WakeAllRigidBodies();
        MeshComp->bBlendPhysics = true;
    }
}

float ABoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsInvulnerable)
    {
        return 0.f;
    }
    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABoss::NotifyActorBeginOverlap(AActor* OtherActor)
{
    if (OtherActor && OtherActor->ActorHasTag("Bullet"))
    {
        return;
    }

    Super::NotifyActorBeginOverlap(OtherActor);
}

void ABoss::ActivateAttack2Collision()
{
    if (Attack2Collision)
    {
        Attack2Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
}

void ABoss::DeactivateAttack2Collision()
{
    if (Attack2Collision)
    {
        Attack2Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    if (LandImpactParticle)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            LandImpactParticle,
            GetActorLocation(),
            FRotator::ZeroRotator,
            FVector(20.0f)
        );
    }
}

void ABoss::OnAttack2CollisionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
    {
        ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
        if (PlayerCharacter)
        {
            FVector KnockbackDirection = PlayerCharacter->GetActorLocation() - GetActorLocation();
            KnockbackDirection.Z = 0;
            KnockbackDirection.Normalize();

            PlayerCharacter->LaunchCharacter(KnockbackDirection * KnockbackStrength, true, false);

            if (LandImpactParticle && GetWorld())
            {
                UGameplayStatics::SpawnEmitterAtLocation(
                    GetWorld(),
                    LandImpactParticle,
                    PlayerCharacter->GetActorLocation(),
                    FRotator::ZeroRotator,
                    FVector(1.0f)
                );
            }
            float DamageValue = 10.0f;
            UGameplayStatics::ApplyDamage(
                OtherActor,
                DamageValue,
                nullptr,
                this,
                UDamageType::StaticClass()
            );
        }
    }
}
