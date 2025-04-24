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
    MonsterHP = 1000.0f;
    MonsterMaxHP = 1000.0f;
    MonsterAttackDamage = 50.0f;
    MonsterArmor = 10;
    MonsterMoveSpeed = 5000.0f;


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

    // 스킬 2 쉴드 이펙트
    Skill2ShieldNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Skill2ShieldNiagara"));
    Skill2ShieldNiagara->SetupAttachment(RootComponent);
    Skill2ShieldNiagara->bAutoActivate = false;


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


    MonsterType = EMonsterType::Boss;
    GameState = Cast<AEdmundGameState>(UGameplayStatics::GetGameState(GetWorld()));

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
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }

    // Stat
    MonsterMoveSpeed = 1000.0f;
    MonsterHP = MonsterMaxHP = 20000.0f;
    //MonsterMaxHP = 100.0f;
    //MonsterHP = 69.0f;
    MonsterArmor = 15.0f;
    MonsterAttackDamage = 50.0f;

    HpbarUpdate();
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

    if (GetCharacterMovement() && GetCharacterMovement()->MovementMode == MOVE_Walking)
    {
        if (!GetCharacterMovement()->IsMovingOnGround())
        {
            GetCharacterMovement()->SetMovementMode(MOVE_Falling);
        }
    }

    if (!bIsInvulnerable && Skill2ShieldNiagara && Skill2ShieldNiagara->IsActive())
    {
        Skill2ShieldNiagara->Deactivate();
    }

    if (GEngine)
    {
        float Rem1 = GetWorld()->GetTimerManager().GetTimerRemaining(Attack1CooldownHandle);
        float Rem2 = GetWorld()->GetTimerManager().GetTimerRemaining(Attack2CooldownHandle);
        float Rem3 = GetWorld()->GetTimerManager().GetTimerRemaining(Attack3CooldownHandle);
        float Rem4 = GetWorld()->GetTimerManager().GetTimerRemaining(Attack4CooldownHandle);

        int32 NextAttack = 0;
        if (AAIController* AICon = Cast<AAIController>(GetController()))
        {
            if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
            {
                NextAttack = BB->GetValueAsInt(TEXT("NextAttack"));
            }
        }

        FString StateName;
        switch (NextAttack)
        {
        case 0: StateName = TEXT("Idle");    break;
        case 1: StateName = TEXT("Attack1"); break;
        case 2: StateName = TEXT("Attack2"); break;
        case 3: StateName = TEXT("Attack3"); break;
        case 4: StateName = TEXT("Attack4"); break;
        default: StateName = TEXT("Unknown"); break;
        }

        FString DebugMsg = FString::Printf(
            TEXT("Ready: A1=%d A2=%d A3=%d A4=%d  Remain: %.1f/%.1f/%.1f/%.1f  State: %s(%d)"),
            bAttack1Ready ? 1 : 0,
            bAttack2Ready ? 1 : 0,
            bAttack3Ready ? 1 : 0,
            bAttack4Ready ? 1 : 0,
            Rem1, Rem2, Rem3, Rem4,
            *StateName, NextAttack
        );
        GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, DebugMsg);
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

    GetWorldTimerManager().ClearTimer(Attack1CooldownHandle);
    GetWorldTimerManager().ClearTimer(Attack2CooldownHandle);
    GetWorldTimerManager().ClearTimer(Attack3CooldownHandle);
    GetWorldTimerManager().ClearTimer(Attack4CooldownHandle);
}

void ABoss::UpdateAttackCooldown(int32 AttackID)
{
    switch (AttackID)
    {
    case 1:
        bAttack1Ready = false;
        GetWorldTimerManager().SetTimer(
            Attack1CooldownHandle,
            [this]() { bAttack1Ready = true; },
            Attack1_CooldownDuration,
            false
        );
        break;

    case 2:
        bAttack2Ready = false;
        GetWorldTimerManager().SetTimer(
            Attack2CooldownHandle,
            [this]() { bAttack2Ready = true; },
            Attack2_CooldownDuration,
            false
        );
        break;

    case 3:
        bAttack3Ready = false;
        GetWorldTimerManager().SetTimer(
            Attack3CooldownHandle,
            [this]() { bAttack3Ready = true; },
            Attack3_CooldownDuration,
            false
        );
        break;

    case 4:
        bAttack4Ready = false;
        GetWorldTimerManager().SetTimer(
            Attack4CooldownHandle,
            [this]() { bAttack4Ready = true; },
            Attack4_CooldownDuration,
            false
        );
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
    float DamageDealt = 0.f;
    if (!bIsInvulnerable)
    {
        DamageDealt = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
        MonsterHP -= DamageDealt;
        HpbarUpdate();
    }
    return DamageDealt;
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
        FVector SpawnLocation = GetActorLocation();
        float CustomZOffset = -50.0f;
        SpawnLocation.Z += CustomZOffset;

        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            LandImpactParticle,
            GetActorLocation(),
            FRotator::ZeroRotator,
            FVector(10.0f)
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
            FVector CollisionCenter = Attack2Collision ? Attack2Collision->GetComponentLocation() : GetActorLocation();
            FVector Direction = PlayerCharacter->GetActorLocation() - CollisionCenter;
            Direction.Z = 0.0f;

            float Distance = Direction.Size();
            if (Direction.IsNearlyZero())
            {
                Direction = FVector(1.0f, 0.0f, 0.0f);
                Distance = 1.0f;
            }
            Direction.Normalize();

            float ExtraMultiplier = 1.0f;
            const float Threshold = 100.0f;
            if (Distance < Threshold)
            {
                ExtraMultiplier = 2.0f;
            }

            float FinalKnockbackStrength = KnockbackStrength * ExtraMultiplier;
            PlayerCharacter->LaunchCharacter(Direction * FinalKnockbackStrength, true, false);

            if (LandImpactParticle && GetWorld())
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                    GetWorld(),
                    LandImpactParticle,
                    PlayerCharacter->GetActorLocation(),
                    FRotator::ZeroRotator,
                    FVector(1.0f)
                );
            }

            float DamageValue = MonsterAttackDamage * Attack2Multiplier;
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

    //if (GameState)
    //{
    //    GameState->PlayMonsterSound(CurrentAudioComp, GetMonsterType(), ESoundType::Attack3);
    //}

    ABoss_Attack1_Bullet* Bullet = ABoss_Attack1_Bullet::GetBulletFromPool(GetWorld(), Attack1BulletClass);
    if (Bullet)
    {
        Bullet->FireProjectile(SpawnLocation, TargetRotation, Direction);
    }

    FTimerHandle ResetRotationTimer;
    GetWorldTimerManager().SetTimer(ResetRotationTimer, this, &ABoss::EnableRotation, 1.0f, false);
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


void ABoss::SetSkill2Invulnerable(bool NewIsInvulnerable)
{
    bIsInvulnerable = NewIsInvulnerable;

    if (NewIsInvulnerable)
    {
        Skill2InvulnerableStartHP = MonsterHP;
        GetWorldTimerManager().SetTimer(Skill2HealingTimerHandle, this, &ABoss::Skill2HealOverTime, Skill2HealingInterval, true);

        if (Skill2ShieldNiagara)
        {
            Skill2ShieldNiagara->Activate(true);
        }
    }
    else
    {
        Skill2InvulnerableStartHP = 0.0f;
        GetWorldTimerManager().ClearTimer(Skill2HealingTimerHandle);
    }
}

void ABoss::Skill2HealOverTime()
{
    if (!bIsInvulnerable)
    {
        GetWorldTimerManager().ClearTimer(Skill2HealingTimerHandle);
        return;
    }
    float MaxHealAmount = MonsterMaxHP * (Skill2MaxHealingPercent / 100.0f);
    float AlreadyHealed = MonsterHP - Skill2InvulnerableStartHP;
    if (AlreadyHealed < MaxHealAmount)
    {
        float HealAmount = MonsterMaxHP * (Skill2HealingPercentPerInterval / 100.0f);
        MonsterHP = FMath::Min(MonsterHP + HealAmount, Skill2InvulnerableStartHP + MaxHealAmount);

        int32 ConvertedMaxHp = FMath::RoundToInt(MonsterMaxHP);
        int32 ConvertedCurrentHp = FMath::RoundToInt(MonsterHP);
        if (AEdmundGameState* MyGameState = GetWorld()->GetGameState<AEdmundGameState>())
        {
            MyGameState->NotifyBossHp(ConvertedMaxHp, ConvertedCurrentHp);
        }
        HpbarUpdate();
    }
}

void ABoss::HpbarUpdate()
{
    int32 ConvertedMaxHp = FMath::RoundToInt(MonsterMaxHP);
    int32 ConvertedCurrentHp = FMath::RoundToInt(MonsterHP);

    if (AEdmundGameState* MyGameState = GetWorld()->GetGameState<AEdmundGameState>())
    {
        MyGameState->NotifyBossHp(ConvertedMaxHp, ConvertedCurrentHp);
    }
}



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
    float WeakenFactor = 0.8f;

    MonsterMaxHP *= WeakenFactor;
    MonsterHP = FMath::Min(MonsterHP, MonsterMaxHP); 
    MonsterAttackDamage *= WeakenFactor;
    MonsterArmor *= WeakenFactor;

    HpbarUpdate();
}


/******** Sounds ******/
void ABoss::BossFireballSounds()
{
    if (GameState && CurrentAudioComp)
    {
        MonsterType = EMonsterType::Boss;
        GameState->PlayMonsterSound(CurrentAudioComp, MonsterType, ESoundType::Attack);
    }
}

void ABoss::BossWingSounds()
{
    if (GameState && CurrentAudioComp)
    {
        MonsterType = EMonsterType::Boss;
        GameState->PlayMonsterSound(CurrentAudioComp, MonsterType, ESoundType::Reload);
    }
}

void ABoss::BossAttack3_1Sounds()
{
    if (GameState && CurrentAudioComp)
    {
        MonsterType = EMonsterType::Boss;
        GameState->PlayMonsterSound(CurrentAudioComp, MonsterType, ESoundType::Attack3);
    }
}

void ABoss::BossAttack3_2Sounds()
{
    if (GameState && CurrentAudioComp)
    {
        MonsterType = EMonsterType::Boss;
        GameState->PlayMonsterSound(CurrentAudioComp, MonsterType, ESoundType::MeleeAttack);
    }
}

//

void ABoss::BossLightLandingSounds()
{
    if (GameState && CurrentAudioComp)
    {
        MonsterType = EMonsterType::Boss;
        GameState->PlayMonsterSound(CurrentAudioComp, MonsterType, ESoundType::Avoid);
    }
}


void ABoss::BossIntroRoarSounds()
{
    if (GameState && CurrentAudioComp)
    {
        MonsterType = EMonsterType::Boss;
        GameState->PlayMonsterSound(CurrentAudioComp, MonsterType, ESoundType::Respawn);
    }
}


void ABoss::BossSkill3RoarSounds()
{
    if (GameState && CurrentAudioComp)
    {
        GameState = Cast<AEdmundGameState>(UGameplayStatics::GetGameState(GetWorld()));
        MonsterType = EMonsterType::Boss;
        GameState->PlayMonsterSound(CurrentAudioComp, MonsterType, ESoundType::Weapon);
    }
}

void ABoss::FreezeMonster(float FreezeDuration)
{
    
}
