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
}

void ABossAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

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

    if (BossCharacter->IsSkill2Invulnerable())
    {
        BBComp->SetValueAsInt("NextAttack", 102);
        return;
    }

    FVector BossForward = BossCharacter->GetActorForwardVector();
    FVector PlayerLocation = GetPlayerLocation();
    FVector DirectionToPlayer = PlayerLocation - BossCharacter->GetActorLocation();
    DirectionToPlayer.Z = 0;
    DirectionToPlayer.Normalize();
    float DotProduct = FVector::DotProduct(BossForward, DirectionToPlayer);
    float AngleInRadians = FMath::Acos(DotProduct);
    float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);

    if (AngleInDegrees > 10.0f || AngleInDegrees < -10.0f)
    {
        BBComp->SetValueAsInt("NextAttack", 0);
        return;
    }

    float Weight1 = ComputeAttack1Weight();
    float Weight2 = ComputeAttack2Weight();
    float Weight3 = ComputeAttack3Weight();
    float Weight4 = ComputeAttack4Weight();

    float MaxWeight = FMath::Max(Weight1, FMath::Max(Weight2, FMath::Max(Weight3, Weight4)));
    int32 NextAttack = 0;
    if (MaxWeight == Weight4)
    {
        NextAttack = 4;
    }
    else if (MaxWeight == Weight3)
    {
        NextAttack = 3;
    }
    else if (MaxWeight == Weight2)
    {
        NextAttack = 2;
    }
    else if (MaxWeight == Weight1)
    {
        NextAttack = 1;
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
}

bool ABossAIController::CheckHpPattern()
{
    float HPPercent = 100.f;
    bool bS1Used = BossCharacter->GetbSkill1Used();
    bool bS2Used = BossCharacter->GetbSkill2Used();
    bool bS3Used = BossCharacter->GetbSkill3Used();

    if (BossCharacter->GetMonsterMaxHP() > 0.f)
    {
        HPPercent = (BossCharacter->GetMonsterHP() / BossCharacter->GetMonsterMaxHP()) * 100.f;
    }
    //if (!bS2Used && HPPercent <= 50.f)
    //{
    //    BBComp->SetValueAsInt("NextAttack", 102);
    //    EnableHalfPattern();
    //    return true;
    //}
    //else
        if (!bS3Used && HPPercent <= 25.f)
    {
        BBComp->SetValueAsInt("NextAttack", 103);
        return true;
    }
    else if (!bS1Used && HPPercent <= 75.f)
    {
        BBComp->SetValueAsInt("NextAttack", 101);
        return true;
    }
    
    return false;
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
    BossCharacter->SetSkill2Invulnerable(true);
    BossCharacter->SetbIsInvulnerable(true);

    //if (BossCharacter->Skill2ShieldNiagara)
    //{
    //    BossCharacter->Skill2ShieldNiagara->Activate(true);
    //}

    MissionHandle->RequestSpawnToSpawnerHandle();
}


void ABossAIController::NotifyClearHalfPattern()
{
    BossCharacter->SetbSkill2Used(true);
    BossCharacter->SetbIsInvulnerable(false);
}

float ABossAIController::ComputeAttack1Weight()
{
    AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player || !BossCharacter) return 0.0f;

    if (BossCharacter->IsSkill2Invulnerable())
    {
        return 0.0f;
    }

    float Distance = FVector::Dist(BossCharacter->GetActorLocation(), Player->GetActorLocation());
    float Weight = 0.0f;
    if (Distance >= 800.f && Distance <= 1500.f)
    {
        Weight = 1.0f;
    }
    else
    {
        Weight = FMath::Clamp(1.0f - FMath::Abs(Distance - 1150.f) / 1150.f, 0.0f, 1.0f);
    }
    if (BossCharacter->Attack1_CooldownEnd > GetWorld()->GetTimeSeconds())
    {
        Weight = 0.0f;
    }
    Weight *= FMath::RandRange(0.8f, 1.2f);
    return Weight;
}

float ABossAIController::ComputeAttack2Weight()
{
    AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player || !BossCharacter) return 0.0f;
    float Distance = FVector::Dist(BossCharacter->GetActorLocation(), Player->GetActorLocation());
    float Weight = 0.0f;
    if (Distance > 1500.f)
    {
        Weight = 1.0f;
    }
    else
    {
        Weight = FMath::Clamp(Distance / 1500.f, 0.0f, 1.0f);
    }
    if (BossCharacter->Attack2_CooldownEnd > GetWorld()->GetTimeSeconds())
    {
        Weight = 0.0f;
    }
    Weight *= FMath::RandRange(0.8f, 1.2f);
    return Weight;
}

float ABossAIController::ComputeAttack3Weight()
{
    AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player || !BossCharacter) return 0.0f;
    float Distance = FVector::Dist(BossCharacter->GetActorLocation(), Player->GetActorLocation());
    float Weight = 0.0f;
    if (Distance < 800.f)
    {
        Weight = 1.0f;
    }
    else
    {
        Weight = FMath::Clamp(1.0f - (Distance - 800.f) / 800.f, 0.0f, 1.0f);
    }
    if (BossCharacter->Attack3_CooldownEnd > GetWorld()->GetTimeSeconds())
    {
        Weight = 0.0f;
    }
    Weight *= FMath::RandRange(0.8f, 1.2f);
    return Weight;
}

float ABossAIController::ComputeAttack4Weight()
{
    AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player || !BossCharacter) return 0.0f;
    float Distance = FVector::Dist(BossCharacter->GetActorLocation(), Player->GetActorLocation());
    float Weight = 0.0f;
    if (Distance >= 1000.f && Distance <= 2000.f)
    {
        Weight = 1.0f;
    }
    else
    {
        Weight = FMath::Clamp(1.0f - FMath::Abs(Distance - 1500.f) / 1500.f, 0.0f, 1.0f);
    }
    if (BossCharacter->Attack4_CooldownEnd > GetWorld()->GetTimeSeconds())
    {
        Weight = 0.0f;
    }
    Weight *= FMath::RandRange(0.8f, 1.2f);
    return Weight;
}

FVector ABossAIController::GetPlayerLocation()
{
    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), PlayerActors);

    if (PlayerActors.Num() > 0)
    {
        return PlayerActors[0]->GetActorLocation();
    }

    return FVector::ZeroVector;
}
