#include "Boss/Boss.h"
#include "Boss/BossState.h"
#include "Boss/BossAIController.h"
#include "Boss/State/Boss_Idle.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "System/MissionHandle.h"

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

    // 공격 3 근접
    Attack2_MeleeCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Attack2_Melee_Range"));
    Attack2_MeleeCollision->SetupAttachment(GetMesh());
    Attack2_MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    // 캡슐
#pragma region Capsule Components Creation

    BossHeadCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BossHeadCapsuleComponent"));
    BossHeadCapsuleComponent->SetupAttachment(GetMesh(), TEXT("BossHead"));

    NeckCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("NeckCapsuleComponent"));
    NeckCapsuleComponent->SetupAttachment(GetMesh(), TEXT("Neck"));

    Front_Right_FootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Front_Right_FootCapsuleComponent"));
    Front_Right_FootCapsuleComponent->SetupAttachment(GetMesh(), TEXT("Front_Right_Foot"));

    Front_Left_FootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Front_Left_FootCapsuleComponent"));
    Front_Left_FootCapsuleComponent->SetupAttachment(GetMesh(), TEXT("Front_Left_Foot"));

    Back_Right_FootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Back_Right_FootCapsuleComponent"));
    Back_Right_FootCapsuleComponent->SetupAttachment(GetMesh(), TEXT("Back_Right_Foot"));

    Back_Left_FootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Back_Left_FootCapsuleComponent"));
    Back_Left_FootCapsuleComponent->SetupAttachment(GetMesh(), TEXT("Back_Left_Foot"));

    Front_Right_LegCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Front_Right_LegCapsuleComponent"));
    Front_Right_LegCapsuleComponent->SetupAttachment(GetMesh(), TEXT("Front_Right_Leg"));

    Front_Left_LegCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Front_Left_LegCapsuleComponent"));
    Front_Left_LegCapsuleComponent->SetupAttachment(GetMesh(), TEXT("Front_Left_Leg"));

    Back_Right_LegCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Back_Right_LegCapsuleComponent"));
    Back_Right_LegCapsuleComponent->SetupAttachment(GetMesh(), TEXT("Back_Right_Leg"));

    Back_Left_LegCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Back_Left_LegCapsuleComponent"));
    Back_Left_LegCapsuleComponent->SetupAttachment(GetMesh(), TEXT("Back_Left_Leg"));

    Body1_CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body1_CapsuleComponent"));
    Body1_CapsuleComponent->SetupAttachment(GetMesh(), TEXT("Body1"));

    Body2_CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body2_CapsuleComponent"));
    Body2_CapsuleComponent->SetupAttachment(GetMesh(), TEXT("Body2"));

    Body3_CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body3_CapsuleComponent"));
    Body3_CapsuleComponent->SetupAttachment(GetMesh(), TEXT("Body3"));

    Tail1CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Tail1CapsuleComponent"));
    Tail1CapsuleComponent->SetupAttachment(GetMesh(), TEXT("Tail1"));

    Tail2CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Tail2CapsuleComponent"));
    Tail2CapsuleComponent->SetupAttachment(GetMesh(), TEXT("Tail2"));

    Tail3CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Tail3CapsuleComponent"));
    Tail3CapsuleComponent->SetupAttachment(GetMesh(), TEXT("Tail3"));

#pragma endregion
    

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

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }

    InitiallizeBullerPool();
    BossController = Cast<ABossAIController>(GetOwner());
    MonsterMoveSpeed = 5000.0f;
    //MonsterHP = 500.0f;
    MonsterHP = MonsterMaxHP = 2000.0f;
    MonsterAttackDamage = 10.0f;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        UE_LOG(LogTemp, Warning, TEXT("Boss 초기 이동 모드 설정: MOVE_Walking"));
    }

    
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
#pragma region Soket
    if (GetMesh())
    {
        if (BossHeadCapsuleComponent)
        {
            BossHeadCapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("BossHead")));
        }
        if (NeckCapsuleComponent)
        {
            NeckCapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Neck")));
        }
        if (Front_Right_FootCapsuleComponent)
        {
            Front_Right_FootCapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Front_Right_Foot")));
        }
        if (Front_Left_FootCapsuleComponent)
        {
            Front_Left_FootCapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Front_Left_Foot")));
        }
        if (Back_Right_FootCapsuleComponent)
        {
            Back_Right_FootCapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Back_Right_Foot")));
        }
        if (Back_Left_FootCapsuleComponent)
        {
            Back_Left_FootCapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Back_Left_Foot")));
        }
        if (Front_Right_LegCapsuleComponent)
        {
            Front_Right_LegCapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Front_Right_Leg")));
        }
        if (Front_Left_LegCapsuleComponent)
        {
            Front_Left_LegCapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Front_Left_Leg")));
        }
        if (Back_Right_LegCapsuleComponent)
        {
            Back_Right_LegCapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Back_Right_Leg")));
        }
        if (Back_Left_LegCapsuleComponent)
        {
            Back_Left_LegCapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Back_Left_Leg")));
        }
        if (Body1_CapsuleComponent)
        {
            Body1_CapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Body1")));
        }
        if (Body2_CapsuleComponent)
        {
            Body2_CapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Body2")));
        }
        if (Body3_CapsuleComponent)
        {
            Body3_CapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Body3")));
        }
        if (Tail1CapsuleComponent)
        {
            Tail1CapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Tail1")));
        }
        if (Tail2CapsuleComponent)
        {
            Tail2CapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Tail2")));
        }
        if (Tail3CapsuleComponent)
        {
            Tail3CapsuleComponent->SetWorldTransform(GetMesh()->GetSocketTransform(TEXT("Tail3")));
        }
    }
#pragma endregion
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

    MissionHandle->CompleteMission();
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

void ABoss::DisableMovement()
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->StopMovementImmediately();
        GetCharacterMovement()->DisableMovement();
    }
}

void ABoss::DisableRotation()
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bOrientRotationToMovement = false;
        GetCharacterMovement()->bUseControllerDesiredRotation = false;
    }

    bUseControllerRotationYaw = false;

    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        AIController->SetControlRotation(GetActorRotation());
    }
}

void ABoss::EnableMovement()
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }
}

void ABoss::EnableRotation()
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bOrientRotationToMovement = true;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
    }

    FRotator CurrentRotation = GetActorRotation();

    FRotator NewRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, 0.0f);
    SetActorRotation(NewRotation);

    bUseControllerRotationYaw = true;
}


void ABoss::FireBullet()
{
    if (!GetWorld() || !MuzzleLocation || !Attack1BulletClass)
    {
        return;
    }

    FVector SpawnLocation = MuzzleLocation->GetComponentLocation();
    AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player)
    {
        return;
    }

    FVector PlayerLocation = Player->GetActorLocation();
    FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();
    FRotator TargetRotation = Direction.Rotation();
    FRotator CurrentRotation = GetActorRotation();
    FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 2.0f);

    SmoothRotation.Pitch = 0.0f;
    SmoothRotation.Roll = 0.0f;

    SetActorRotation(SmoothRotation);
    DisableRotation();
    TargetRotation = (PlayerLocation - SpawnLocation).Rotation();

    ABoss_Attack1_Bullet* Bullet = ABoss_Attack1_Bullet::GetBulletFromPool(GetWorld(), Attack1BulletClass);
    if (Bullet)
    {
        Bullet->FireProjectile(SpawnLocation, TargetRotation, Direction);
    }

    FTimerHandle ResetRotationTimer;
    GetWorldTimerManager().SetTimer(ResetRotationTimer, this, &ABoss::EnableRotation, 1.0f, false); // 1초 후 회전 다시 활성화
}

void ABoss::HandleAttack2State(int32 State)
{

    switch (State)
    {
    case 1:
        GetCharacterMovement()->SetMovementMode(MOVE_Flying);
        LaunchCharacter(FVector(0, 0, 1000), false, false);
        break;

    case 2:
        GetCharacterMovement()->StopMovementImmediately();
        GetCharacterMovement()->GravityScale = 0.0f;
        break;

    case 3:
        GetCharacterMovement()->GravityScale = 1.0f;
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        break;
    }
}

void ABoss::OnAttack2Finished()
{
    if (BTTask_BossAttack2Instance)
    {
        BTTask_BossAttack2Instance->OnAttack2Completed();
    }
}


//void ABoss::SetSkill2Invulnerable(bool NewIsInvulnerable)
//{
//    if (bIsInvulnerable)
//    {
//        Skill2InvulnerableStartHP = MonsterHP;
//        GetWorldTimerManager().SetTimer(Skill2HealingTimerHandle, this, &ABoss::Skill2HealOverTime, Skill2HealingInterval, true);
//    }
//    else
//    {
//        GetWorldTimerManager().ClearTimer(Skill2HealingTimerHandle);
//        Skill2InvulnerableStartHP = 0.0f;
//    }
//}
//
//void ABoss::Skill2HealOverTime()
//{
//    if (!bIsInvulnerable)
//    {
//        GetWorldTimerManager().ClearTimer(Skill2HealingTimerHandle);
//        return;
//    }
//    float MaxHealAmount = MonsterMaxHP * (Skill2MaxHealingPercent / 100.0f);
//    float AlreadyHealed = MonsterHP - Skill2InvulnerableStartHP;
//    if (AlreadyHealed < MaxHealAmount)
//    {
//        float HealAmount = MonsterMaxHP * (Skill2HealingPercentPerInterval / 100.0f);
//        MonsterHP = FMath::Min(MonsterHP + HealAmount, Skill2InvulnerableStartHP + MaxHealAmount);
//    }
//}


void ABoss::InitBoss(AMissionHandle* NewMissionHandle)
{
    BossController->InitBlackboard(NewMissionHandle);
    MissionHandle = NewMissionHandle;
    CheckWeaken();
}

void ABoss::CheckWeaken()
{
    if (!IsValid(MissionHandle))
    {
        return;
    }

    if (MissionHandle->GetWeakenBoss())
    {
        ApplyWeaken();
    }
}

void ABoss::ApplyWeaken()
{
    // TODO : 보스 약화 구현 필요
}

