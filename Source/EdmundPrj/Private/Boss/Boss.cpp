#include "Boss/Boss.h"
#include "Boss/BossState.h"
#include "Boss/State/Boss_Idle.h"
#include "Boss/State/Boss_Chase.h"
#include "Boss/State/Boss_Attack1.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ABoss::ABoss()
{
    PrimaryActorTick.bCanEverTick = false;
    BossState = nullptr;

    MonsterMoveSpeed = 500.0f;

    MuzzleLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(GetMesh(), TEXT("MuzzleSocket"));
    
}

void ABoss::BeginPlay()
{
    Super::BeginPlay();

    if (GetMesh() && GetMesh()->GetAnimInstance())
    {
        AnimInstance = Cast<UBoss_AnimInstance>(GetMesh()->GetAnimInstance());
    }
    InitiallizeBullerPool();
}

void ABoss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = MonsterChaseSpeed;
    }
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
        BossState = NewObject<UBoss_Idle>(this);
        if (AnimInstance) AnimInstance->bIsMoving = false;
        break;

    case EBossState::Chase:
        BossState = NewObject<UBoss_Chase>();
        GetCharacterMovement()->MaxWalkSpeed = MonsterMoveSpeed;
        if (AnimInstance) AnimInstance->bIsMoving = true;
        break;

    case EBossState::Attack1:
        BossState = NewObject<UBoss_Attack1>();
        break;

    default:
        BossState = nullptr;
        break;
    }

    if (BossState)
    {
        BossState->EnterState(this);
    }
}

void ABoss::InitiallizeBullerPool()
{
    if (!GetWorld()) return;

    int32 PoolSize = 5;

    for (int32 i = 0; i < PoolSize; ++i)
    {
        ABoss_Attack1_Bullet* Bullet = GetWorld()->SpawnActor<ABoss_Attack1_Bullet>(Attack1BulletClass);
        if (Bullet)
        {
            Bullet->SetActorHiddenInGame(true);
            Bullet->SetActorEnableCollision(false);
        }
    }
}

void ABoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    ABoss_Attack1_Bullet::BulletPool.Empty();
}