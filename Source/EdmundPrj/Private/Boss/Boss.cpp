#include "Boss/Boss.h"
#include "Boss/BossState.h"
#include "Boss/BossAIController.h"
#include "Boss/State/Boss_Idle.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ABoss::ABoss()
{
    PrimaryActorTick.bCanEverTick = true;

    AIControllerClass = ABossAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    BossState = nullptr;
    MonsterMoveSpeed = 500.0f;
    MonsterHP = 50.0f;
    MonsterMaxHP = 50.0f;

    // 탄환 발사 위치
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
        GetCharacterMovement()->MaxWalkSpeed = MonsterMoveSpeed;
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