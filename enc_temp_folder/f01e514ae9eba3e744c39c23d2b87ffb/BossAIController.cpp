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
            // --- [ 스킬 우선 로직 ] ---

            // 체력 퍼센트 계산 (0~100)
            float HPPercent = 100.f;
            if (BossCharacter->GetMonsterMaxHP() > 0.f)
            {
                HPPercent = (BossCharacter->GetMonsterMaxHP() / BossCharacter->GetMonsterMaxHP()) * 100.f;
            }

            // 이미 스킬을 썼는지 여부 (한 번만 실행)
            bool bS1Used = BossCharacter->GetbSkill1Used();
            bool bS2Used = BossCharacter->GetbSkill2Used();
            bool bS3Used = BossCharacter->GetbSkill3Used();

            // 체력 조건: 75% 이하 스킬1, 50% 이하 스킬2, 25% 이하 스킬3
            // 우선순위: 스킬3 > 스킬2 > 스킬1
            int32 NextAttack = 0; // 0이면 공격로직

            if (!bS3Used && HPPercent <= 25.f)
            {
                NextAttack = 103; // 스킬3
            }
            else if (!bS2Used && HPPercent <= 50.f)
            {
                NextAttack = 102; // 스킬2
            }
            else if (!bS1Used && HPPercent <= 75.f)
            {
                NextAttack = 101; // 스킬1
            }

            // 만약 NextAttack이 101~103(스킬)이면 공격로직 스킵,
            // 아니면 기존 로직 실행
            if (NextAttack == 0)
            {
                // --------------------------
                //      [ 기존 코드 ]
                // --------------------------
                float CurrentTime = GetWorld()->GetTimeSeconds();
                bool bAttack1Ready = (CurrentTime >= BossCharacter->Attack1_CooldownEnd);
                bool bAttack2Ready = (CurrentTime >= BossCharacter->Attack2_CooldownEnd);
                bool bAttack3Ready = (CurrentTime >= BossCharacter->Attack3_CooldownEnd);
                bool bAttack4Ready = (CurrentTime >= BossCharacter->Attack4_CooldownEnd);

                AActor* Player = UGameplayStatics::GetPlayerPawn(BossCharacter->GetWorld(), 0);
                float Distance = 10000.0f;
                if (Player)
                {
                    Distance = FVector::Dist(BossCharacter->GetActorLocation(), Player->GetActorLocation());
                }

                // 플레이어와 500 이하이면 Attack3의 가중치 * 1.5
                // 플레이어와 1500 이상이면 Attack2의 가중치 * 2
                float meleeModifier = (Distance <= 500.f) ? 1.5f : 1.0f;
                float rangedModifier = (Distance >= 1500.f) ? 2.0f : 1.0f;

                float finalWeight1 = bAttack1Ready ? 1.0f : 0.0f;
                float finalWeight2 = bAttack2Ready ? (1.0f * rangedModifier) : 0.0f;
                float finalWeight3 = bAttack3Ready ? (1.0f * meleeModifier) : 0.0f;
                float finalWeight4 = bAttack4Ready ? 1.0f : 0.0f;

                float maxWeight = FMath::Max(
                    finalWeight1,
                    FMath::Max(finalWeight2, FMath::Max(finalWeight3, finalWeight4))
                );

                int32 AttackID = 0; // 기본값
                if (maxWeight == finalWeight4)      AttackID = 4;
                else if (maxWeight == finalWeight3) AttackID = 3;
                else if (maxWeight == finalWeight2) AttackID = 2;
                else if (maxWeight == finalWeight1) AttackID = 1;
                else                                 AttackID = 0;

                // NextAttack = AttackID
                NextAttack = AttackID;

                // 남은 쿨타임 계산 (음수가 되지 않도록 처리)
                float Attack1Remaining = FMath::Max(0.0f, BossCharacter->Attack1_CooldownEnd - CurrentTime);
                float Attack2Remaining = FMath::Max(0.0f, BossCharacter->Attack2_CooldownEnd - CurrentTime);
                float Attack3Remaining = FMath::Max(0.0f, BossCharacter->Attack3_CooldownEnd - CurrentTime);
                float Attack4Remaining = FMath::Max(0.0f, BossCharacter->Attack4_CooldownEnd - CurrentTime);

                FString DebugMsg = FString::Printf(
                    TEXT("A1(%.2f): %.0f, A2(%.2f): %.0f, A3(%.2f): %.0f, A4(%.2f): %.0f, Next: %d"),
                    finalWeight1, Attack1Remaining,
                    finalWeight2, Attack2Remaining,
                    finalWeight3, Attack3Remaining,
                    finalWeight4, Attack4Remaining,
                    NextAttack
                );
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(1, DeltaTime, FColor::Green, DebugMsg);
                }
            }
            else
            {
                // 스킬(101~103)인 경우 디버그 출력
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(
                        99,
                        DeltaTime,
                        FColor::Red,
                        FString::Printf(TEXT("Skill triggered!! HP=%.1f => NextAttack=%d"), HPPercent, NextAttack)
                    );
                }
            }

            // Blackboard에 NextAttack 기록
            BBComp->SetValueAsInt("NextAttack", NextAttack);
        }
    }
}

