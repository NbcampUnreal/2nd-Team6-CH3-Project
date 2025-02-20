#include "Boss/Attack2/BTT_BossAttack2.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTT_BossAttack2::UBTT_BossAttack2()
{
    NodeName = "Boss Attack 2";
}

EBTNodeResult::Type UBTT_BossAttack2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        return EBTNodeResult::Failed;
    }

    AActor* BossActor = AICon->GetPawn();
    if (!BossActor)
    {
        return EBTNodeResult::Failed;
    }

    GetWorld()->GetTimerManager().SetTimer(GroundWaitTimerHandle, FTimerDelegate::CreateLambda([this, BossActor, &OwnerComp]()
        {
            StartRising(BossActor, &OwnerComp);
        }), 2.0f, false); // 바닥 대기

    return EBTNodeResult::InProgress;
}

void UBTT_BossAttack2::StartRising(AActor* BossActor, UBehaviorTreeComponent* OwnerComp)
{
    StartLocation = BossActor->GetActorLocation();
    RiseTarget = StartLocation + FVector(0, 0, RiseHeight);
    RiseAlpha = 0.0f;

    GetWorld()->GetTimerManager().SetTimer(RiseTimerHandle, FTimerDelegate::CreateLambda([this, BossActor, OwnerComp]()
        {
            UpdateRising(BossActor, OwnerComp);
        }), 0.016f, true);
}

void UBTT_BossAttack2::UpdateRising(AActor* BossActor, UBehaviorTreeComponent* OwnerComp)
{
    if (RiseAlpha < 1.0f)
    {
        RiseAlpha += GetWorld()->GetDeltaSeconds() * RiseSpeed;
        float NewZ = FMath::InterpEaseInOut(StartLocation.Z, RiseTarget.Z, RiseAlpha, 2.0f);
        BossActor->SetActorLocation(FVector(StartLocation.X, StartLocation.Y, NewZ));
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(RiseTimerHandle);
        ACharacter* BossCharacter = Cast<ACharacter>(BossActor);
        if (BossCharacter)
        {
            BossCharacter->GetCharacterMovement()->GravityScale = 0.0f;
        }

        AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (Player)
        {
            FVector PlayerLocation = Player->GetActorLocation();
            BossActor->SetActorLocation(FVector(PlayerLocation.X, PlayerLocation.Y, RiseTarget.Z));
        }

        StartAirWait(BossActor, OwnerComp);
    }
}

void UBTT_BossAttack2::StartAirWait(AActor* BossActor, UBehaviorTreeComponent* OwnerComp)
{
    GetWorld()->GetTimerManager().SetTimer(AirWaitTimerHandle, FTimerDelegate::CreateLambda([this, BossActor, OwnerComp]()
        {
            StartFalling(BossActor, OwnerComp);
        }), 2.0f, false); // 공중 대기
}

void UBTT_BossAttack2::StartFalling(AActor* BossActor, UBehaviorTreeComponent* OwnerComp)
{
    FVector FallStartLocation = BossActor->GetActorLocation();
    FVector FallTarget = FVector(FallStartLocation.X, FallStartLocation.Y, StartLocation.Z); // 원래 Z로 하강

    // 하강 시작 시 중력 다시 활성화
    ACharacter* BossCharacter = Cast<ACharacter>(BossActor);
    if (BossCharacter)
    {
        BossCharacter->GetCharacterMovement()->GravityScale = 1.0f;
    }

    GetWorld()->GetTimerManager().SetTimer(FallTimerHandle, FTimerDelegate::CreateLambda([this, BossActor, FallStartLocation, FallTarget, OwnerComp]()
        {
            UpdateFalling(BossActor, FallStartLocation, FallTarget, OwnerComp);
        }), 0.016f, true);
}

void UBTT_BossAttack2::UpdateFalling(AActor* BossActor, FVector FallStartLocation, FVector FallTarget, UBehaviorTreeComponent* OwnerComp)
{
    if (FallAlpha < 1.0f)
    {
        FallAlpha += GetWorld()->GetDeltaSeconds() * FallSpeed;
        float NewZ = FMath::InterpEaseInOut(FallStartLocation.Z, FallTarget.Z, FallAlpha, 3.0f);
        BossActor->SetActorLocation(FVector(FallTarget.X, FallTarget.Y, NewZ));
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(FallTimerHandle);
        FinishTask(OwnerComp);
    }
}

void UBTT_BossAttack2::FinishTask(UBehaviorTreeComponent* OwnerComp)
{
    if (OwnerComp)
    {
        OwnerComp->OnTaskFinished(this, EBTNodeResult::Succeeded);
    }
}
