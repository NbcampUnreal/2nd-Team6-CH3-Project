#include "Boss/Boss.h"
#include "Boss/BossState.h"
#include "Boss/State/Boss_Idle.h"
#include "Boss/State/Boss_Chase.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ABoss::ABoss()
{
    PrimaryActorTick.bCanEverTick = false;
    BossState = nullptr;

    MonsterMoveSpeed = 500.0f;
}

void ABoss::BeginPlay()
{
    Super::BeginPlay();
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
        break;
    case EBossState::Chase:
        BossState = NewObject<UBoss_Chase>();
        GetCharacterMovement()->MaxWalkSpeed = MonsterMoveSpeed;
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
