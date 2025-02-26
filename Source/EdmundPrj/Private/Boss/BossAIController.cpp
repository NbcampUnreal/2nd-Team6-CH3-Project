#include "Boss/BossAIController.h"
#include "Boss/Boss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

ABossAIController::ABossAIController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABossAIController::BeginPlay()
{
    Super::BeginPlay();

    BossCharacter = Cast<ABoss>(GetPawn());
    if (!BossCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("BossAIController::BeginPlay - BossCharacter is null"));
    }
}

void ABossAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    BossCharacter = Cast<ABoss>(InPawn);
    if (!BossCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("BossAIController::OnPossess - BossCharacter is null"));
    }
    else
    {

    }
}

void ABossAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (BossCharacter)
    {
        UBlackboardComponent* BBComp = GetBlackboardComponent();
        if (BBComp)
        {
            float CurrentTime = GetWorld()->GetTimeSeconds();
            bool bAttack1Ready = (CurrentTime >= BossCharacter->Attack1_CooldownEnd);
            bool bAttack2Ready = (CurrentTime >= BossCharacter->Attack2_CooldownEnd);
            bool bAttack3Ready = (CurrentTime >= BossCharacter->Attack3_CooldownEnd);
            bool bAttack4Ready = (CurrentTime >= BossCharacter->Attack4_CooldownEnd);

            int32 NextAttack = 0;

            AActor* Player = UGameplayStatics::GetPlayerPawn(BossCharacter->GetWorld(), 0);
            float MeleeRange = 500.0f;
            if (Player)
            {
                float Distance = FVector::Dist(BossCharacter->GetActorLocation(), Player->GetActorLocation());
                if (Distance <= MeleeRange && bAttack3Ready)
                {
                    NextAttack = 3;
                }
            }

            if (NextAttack == 0)
            {
                if (bAttack4Ready)
                {
                    NextAttack = 4;
                }
                else if (bAttack3Ready)
                {
                    NextAttack = 3;
                }
                else if (bAttack2Ready)
                {
                    NextAttack = 2;
                }
                else if (bAttack1Ready)
                {
                    NextAttack = 1;
                }
                else
                {
                    NextAttack = 0;
                }
            }

            BBComp->SetValueAsInt("NextAttack", NextAttack);
            float Attack1Remaining = FMath::Max(0.0f, BossCharacter->Attack1_CooldownEnd - CurrentTime);
            float Attack2Remaining = FMath::Max(0.0f, BossCharacter->Attack2_CooldownEnd - CurrentTime);
            float Attack3Remaining = FMath::Max(0.0f, BossCharacter->Attack3_CooldownEnd - CurrentTime);
            float Attack4Remaining = FMath::Max(0.0f, BossCharacter->Attack4_CooldownEnd - CurrentTime);

            FString CooldownInfo = FString::Printf(TEXT("A1: %.0f, A2: %.0f, A3: %.0f, A4: %.0f, Next: %d"),
                Attack1Remaining, Attack2Remaining, Attack3Remaining, Attack4Remaining, NextAttack);
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(1, DeltaTime, FColor::Green, CooldownInfo);
            }
        }
    }
}

