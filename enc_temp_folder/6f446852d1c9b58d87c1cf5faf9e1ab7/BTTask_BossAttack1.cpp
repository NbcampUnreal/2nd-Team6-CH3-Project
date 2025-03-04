#include "Boss/State/BTTask_BossAttack1.h"
#include "Boss/Boss.h"
#include "AIController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Boss/Boss_AnimInstance.h"

UBTTask_BossAttack1::UBTTask_BossAttack1()
{
    NodeName = TEXT("Boss Attack1");
    BossRef = nullptr;
    CachedOwnerComp = nullptr;
    bNotifyTick = true; // ✅ TickTask 활성화
}

EBTNodeResult::Type UBTTask_BossAttack1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    BossRef = Cast<ABoss>(AIController->GetPawn());
    if (!BossRef)
    {
        return EBTNodeResult::Failed;
    }

    CachedOwnerComp = &OwnerComp;

    BossRef->DisableMovement();
    BossRef->DisableRotation();

    return EBTNodeResult::InProgress; // ✅ TickTask에서 회전 후 공격하도록 설정
}

void UBTTask_BossAttack1::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (!BossRef)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
    if (!Player)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    FVector PlayerLocation = Player->GetActorLocation();
    FVector BossLocation = BossRef->GetActorLocation();
    FVector DirectionToPlayer = (PlayerLocation - BossLocation).GetSafeNormal();

    // ✅ 부드러운 회전 적용
    FRotator CurrentRotation = BossRef->GetActorRotation();
    FRotator TargetRotation = DirectionToPlayer.Rotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, 3.0f); // 회전 속도 조절 가능

    BossRef->SetActorRotation(NewRotation);

    // ✅ 일정 각도 이하로 차이가 줄어들면 공격 실행
    float AngleDifference = FMath::Abs(FRotator::NormalizeAxis(TargetRotation.Yaw - CurrentRotation.Yaw));
    if (AngleDifference < 5.0f) // 🔥 5도 이하로 차이가 줄어들면 공격 실행
    {
        UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
        if (AnimInst && AnimInst->Attack1Montage)
        {
            float Duration = BossRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimInst->Attack1Montage);
            if (Duration > 0.0f)
            {
                FTimerHandle MontageEndTimer;
                BossRef->GetWorldTimerManager().SetTimer(MontageEndTimer, this, &UBTTask_BossAttack1::OnAnimationCompleted, Duration, false);
            }
        }
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}

void UBTTask_BossAttack1::OnAnimationCompleted()
{
    if (!BossRef || !CachedOwnerComp)
    {
        return;
    }

    BossRef->EnableMovement();
    BossRef->EnableRotation();

    BossRef->UpdateAttackCooldown(1);
    BossRef->SetbChaseComplete(true);

    FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}
