#include "Boss/Boss.h"
#include "Boss/BossState.h"
#include "Boss/State/Boss_Idle.h"
#include "Boss/State/Boss_Chase.h"
#include "Boss/State/Boss_Attack1.h"
#include "Boss/State/Boss_Attack2.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ABoss::ABoss()
{
    // Tick을 사용해야 Attack2 등에서 TickState를 호출할 수 있음
    PrimaryActorTick.bCanEverTick = true;

    BossState = nullptr;
    MonsterMoveSpeed = 500.0f;

    // 총알 발사 위치 컴포넌트 생성
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

    // 현재 상태 업데이트
    if (BossState)
    {
        BossState->TickState(DeltaTime);
    }

    // 예: Chase 상태에서 이동 속도 조정
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = MonsterChaseSpeed;
    }
}

void ABoss::SetState(EBossState NewState)
{
    // 기존 상태 종료
    if (BossState)
    {
        BossState->ExitState();
        BossState = nullptr;
    }

    // 새 상태 생성
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

    case EBossState::Attack2:
        BossState = NewObject<UBoss_Attack2>();
        break;

    default:
        BossState = nullptr;
        break;
    }

    // 새 상태 진입
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

    // Attack1 총알 풀 정리
    ABoss_Attack1_Bullet::BulletPool.Empty();
}
