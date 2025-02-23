#include "Boss/BossAIController.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"

void ABossAIController::BeginPlay()
{
    Super::BeginPlay();
    BossCharacter = Cast<ABoss>(GetPawn());
	APawn* ControllerBoss = GetPawn();

    if (!ControllerBoss)
    {
		UE_LOG(LogTemp, Error, TEXT("BossAIController: ControllerBoss is NULL"));
    }

    if (BossCharacter)
    {
        BossCharacter->SetState(EBossState::Attack2);
    }
}

void ABossAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
