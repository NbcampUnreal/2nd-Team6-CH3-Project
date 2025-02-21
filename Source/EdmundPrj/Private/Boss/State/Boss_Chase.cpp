#include "Boss/State/Boss_Chase.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "GameFramework/Character.h"

void UBoss_Chase::EnterState(ABoss* Boss)
{
    Super::EnterState(Boss);
    UE_LOG(LogTemp, Log, TEXT("Boss is now CHASING"));

    if (!Boss || !Boss->GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("Boss or World is NULL in Chase State"));
        return;
    }
    BossCharacter = Boss;
    Boss->GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, this, &UBoss_Chase::UpdateChase, 0.5f, true);
}

void UBoss_Chase::ExitState()
{
    UE_LOG(LogTemp, Log, TEXT("Boss is exiting CHASE"));

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);
    }
}

void UBoss_Chase::UpdateChase()
{
    if (!BossCharacter || !BossCharacter->GetWorld())
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateChase: BossCharacter or World is NULL"));
        return;
    }

    AAIController* AICon = Cast<AAIController>(BossCharacter->GetController());
    AActor* Player = UGameplayStatics::GetPlayerPawn(BossCharacter->GetWorld(), 0);

    if (!AICon || !Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateChase: AIController or Player is NULL"));
        return;
    }

    AICon->MoveToActor(Player, 1000.0f);
}
