#include "Boss/Boss.h"
#include "Boss/BossAIController.h"
#include "Boss/BossState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

ABoss::ABoss()
{
    PrimaryActorTick.bCanEverTick = true;
    BossState = NewObject<UBossState>();
}

void ABoss::BeginPlay()
{
    Super::BeginPlay();
    if (BossState)
    {
        BossState->Initialize(this);
    }
}

void ABoss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (BossState)
    {
        BossState->UpdateState(DeltaTime);
    }
}