#include "Boss/BossState.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

void UBossState::Initialize(ABoss* Boss)
{
    BossCharacter = Boss;
    CurrentState = EBossState::Idle;
}

void UBossState::UpdateState(float DeltaTime)
{
    if (!BossCharacter) return;

    AActor* Player = UGameplayStatics::GetPlayerPawn(BossCharacter->GetWorld(), 0);
    if (!Player) return;

    float Distance = FVector::Dist(BossCharacter->GetActorLocation(), Player->GetActorLocation());


    if (Distance > 200.0f)
    {
        SetState(EBossState::Chase);
    }
    else
    {
        SetState(EBossState::Idle);
    }
}


void UBossState::SetState(EBossState NewState)
{
    if (CurrentState == NewState) return;
    CurrentState = NewState;

    if (!BossCharacter) return;

    AAIController* AIController = Cast<AAIController>(BossCharacter->GetController());
    if (!AIController)
    {
        return;
    }

    switch (CurrentState)
    {
    case EBossState::Chase:
    {
        AActor* Player = UGameplayStatics::GetPlayerPawn(BossCharacter->GetWorld(), 0);
        if (Player)
        {
            AIController->MoveToActor(Player, 50.0f);
        }
    }
    break;
    case EBossState::Idle:
        AIController->StopMovement();
        break;
    default:
        break;
    }
}
