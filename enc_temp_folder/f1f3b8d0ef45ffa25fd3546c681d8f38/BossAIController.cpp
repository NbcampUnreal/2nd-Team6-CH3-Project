#include "Boss/BossAIController.h"
#include "Boss/Boss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/MissionHandle.h"

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

    if (!InPawn)
    {
        UE_LOG(LogTemp, Error, TEXT("BossAIController::OnPossess - InPawn is NULL!"));
        return;
    }

    BossCharacter = Cast<ABoss>(InPawn);
    if (!BossCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("BossAIController::OnPossess - BossCharacter Cast Failed! InPawn Name: %s"), *InPawn->GetName());
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("BossAIController::OnPossess - Successfully Possessed %s"), *BossCharacter->GetName());
}

void ABossAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsStart)
    {
        return;
    }

    if (!IsValid(BossCharacter))
    {
        return;
    }

    if (!IsValid(BBComp))
    {
        return;
    }

    if (CheckHpPattern())
    {
        return;
    }

    CheckBossHeight();

    int32 NextAttack = 0;

    if (BossCharacter->GetbChaseComplete())
    {
        float CurrentTime = GetWorld()->GetTimeSeconds();

        CheckLockedSkill(CurrentTime);

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
            finalWeight1, FMath::Max(
            finalWeight2, FMath::Max(
            finalWeight3, finalWeight4))
        );

        if (maxWeight == finalWeight4)      NextAttack = 4;
        else if (maxWeight == finalWeight3) NextAttack = 3;
        else if (maxWeight == finalWeight2) NextAttack = 2;
        else if (maxWeight == finalWeight1) NextAttack = 1;
        else                                NextAttack = 0;

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
        NextAttack = 0;
    }

    BBComp->SetValueAsInt("NextAttack", NextAttack);
}

void ABossAIController::InitBlackboard(AMissionHandle* NewMissionHandle)
{
    BBComp = GetBlackboardComponent();
    MissionHandle = NewMissionHandle;
    bIsStart = true;
}

bool ABossAIController::CheckHpPattern() // 체력 퍼센트
{
    float HPPercent = 100.f;
    bool bS1Used = BossCharacter->GetbSkill1Used();
    bool bS2Used = BossCharacter->GetbSkill2Used();
    bool bS3Used = BossCharacter->GetbSkill3Used();

    if (BossCharacter->GetMonsterMaxHP() > 0.f)
    {
        HPPercent = (BossCharacter->GetMonsterHP() / BossCharacter->GetMonsterMaxHP()) * 100.f;
    }

    if (!bS3Used && HPPercent <= 25.f)
    {
        BBComp->SetValueAsInt("NextAttack", 103);
        return true;
    }
    else if (!bS2Used && HPPercent <= 50.f)
    {
        BBComp->SetValueAsInt("NextAttack", 102);
        EnableHalfPattern();
        return true;
    }
    else if (!bS1Used && HPPercent <= 75.f)
    {
        BBComp->SetValueAsInt("NextAttack", 101);
        return true;
    }
    
    return false;
}

void ABossAIController::CheckBossHeight()
{
    bool bIsFalling = BossCharacter->GetCharacterMovement()->IsFalling();
    float BossHeight = BossCharacter->GetActorLocation().Z;
    const float FlyThreshold = 10.0f;

    if (bIsFalling || BossHeight > FlyThreshold)
    {
        BossCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
    }
    else
    {
        BossCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }
}

void ABossAIController::CheckLockedSkill(float CurrentTime)
{
    bAttack1Ready = (CurrentTime >= BossCharacter->Attack1_CooldownEnd);
    bAttack2Ready = (CurrentTime >= BossCharacter->Attack2_CooldownEnd);
    bAttack3Ready = (CurrentTime >= BossCharacter->Attack3_CooldownEnd);
    bAttack4Ready = (CurrentTime >= BossCharacter->Attack4_CooldownEnd);

    switch (MissionHandle->GetLockedSkill())
    {
    case EBossState::Attack1:
        bAttack1Ready = false;
        break;

    case EBossState::Attack2:
        bAttack2Ready = false;
        break;

    case EBossState::Attack3:
        bAttack3Ready = false;
        break;

    case EBossState::Attack4:
        bAttack4Ready = false;
        break;
    }
}

void ABossAIController::EnableHalfPattern()
{
    if (bIsEnableHalf)
    {
        return;
    }

    bIsEnableHalf = true;

    MissionHandle->RequestSpawnToSpawnerHandle();
}

void ABossAIController::NotifyClearHalfPattern()
{
    BossCharacter->SetbSkill2Used(true);
    BossCharacter->SetbIsInvulnerable(false);
}
