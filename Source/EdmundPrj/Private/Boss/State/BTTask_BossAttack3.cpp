#include "Boss/State/BTTask_BossAttack3.h"
#include "Boss/Boss.h"
#include "AIController.h"
#include "Engine/World.h"
#include "Behaviortree/BlackboardComponent.h"
#include "Boss/Boss_AnimInstance.h"
#include "Kismet/GameplayStatics.h"

UBTTask_BossAttack3::UBTTask_BossAttack3()
{
    NodeName = TEXT("Boss Attack3");
    bNotifyTick = false; 
    BossRef = nullptr;
    CachedOwnerComp = nullptr;
    ComboPhase = 0;
}

EBTNodeResult::Type UBTTask_BossAttack3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    CachedOwnerComp = &OwnerComp;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("Boss AIController is NULL"));
        return EBTNodeResult::Failed;
    }

    this->BossRef = Cast<ABoss>(AIController->GetPawn());
    if (!this->BossRef)
    {
        UE_LOG(LogTemp, Error, TEXT("BossRef is NULL in ExecuteTask"));
        return EBTNodeResult::Failed;
    }

    BossRef->SetCurrentAttackTask(this);
    if (!BossRef->CurrentAttackTask)
    {
        UE_LOG(LogTemp, Error, TEXT("CurrentAttackTask is NULL after setting it!"));
        return EBTNodeResult::Failed;
    }

    UE_LOG(LogTemp, Log, TEXT("CurrentAttackTask successfully set: %s"), *BossRef->CurrentAttackTask->GetName());

    PlayAttack3Montage();
    return EBTNodeResult::InProgress;
}



void UBTTask_BossAttack3::OnAttack1Notify()
{
    if (!BossRef)
    {
        UE_LOG(LogTemp, Error, TEXT("OnAttack1Notify: BossRef is NULL!"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("combo %d"), BossRef->GetComboPhase());

    if (ComboPhase != 1)
        return;

    BossRef->SetComboPhase(2);
    ComboPhase = 2;
    ExecuteMeleeAttack();
}


void UBTTask_BossAttack3::OnAttack2Notify()
{
    if (ComboPhase != 2)
        return;

    BossRef->SetComboPhase(3);
    ComboPhase = 3;
    FinishComboAttack();
    UE_LOG(LogTemp, Log, TEXT("combo %d, %d"), BossRef->GetComboPhase(), ComboPhase);

}

void UBTTask_BossAttack3::FinishComboAttack()
{
    if (!BossRef)
    {
        UE_LOG(LogTemp, Error, TEXT("FinishComboAttack: BossRef is NULL!"));
        return;
    }

    BossRef->UpdateAttackCooldown(3);
    BossRef->CurrentAttackTask = nullptr;

    ComboPhase = 0;
    BossRef->SetComboPhase(0);

    if (CachedOwnerComp)
    {
        BossRef->SetbChaseComplete(true);
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("FinishComboAttack: CachedOwnerComp is NULL! Behavior Tree Task could not finish properly."));
    }
}






void UBTTask_BossAttack3::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 애니메이션 노티파이에 의존하므로 Tick에서는 별도의 로직을 처리하지 않습니다.
}

void UBTTask_BossAttack3::PlayAttack3Montage()
{
    if (!BossRef)
    {
        return;
    }

    UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
    if (AnimInst && AnimInst->Attack3Montage)
    {
        BossRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimInst->Attack3Montage);
    }
}



void UBTTask_BossAttack3::ExecuteMeleeAttack()
{
    if (!BossRef)
        return;

    float DashDistance = 0.0f;
    float DashSpeed = 0.0f;

    // 공격 단계에 따른 이동 거리와 속도 설정
    switch (BossRef->GetComboPhase())
    {
    case 1:  // 1타
        DashDistance = BossRef->MeleeAttackDashDistance_Attack1;
        DashSpeed = BossRef->MeleeAttackDashSpeed_Attack1;
        break;
    case 2:  // 2타
        DashDistance = BossRef->MeleeAttackDashDistance_Attack2;
        DashSpeed = BossRef->MeleeAttackDashSpeed_Attack2;
        break;
    }

    // 돌진 방향 설정
    FVector DashDirection = BossRef->GetActorForwardVector();
    FVector LaunchVelocity = DashDirection * DashSpeed;
    BossRef->LaunchCharacter(LaunchVelocity, true, true);

    // 공격 실행
    BossRef->MonsterAttackCheck();

    // 콤보 진행
    if (BossRef->GetComboPhase() < 3)
    {
        int32 NextPhase = BossRef->GetComboPhase() + 1;
        BossRef->SetComboPhase(NextPhase);
        ComboPhase = NextPhase; 
    }
}


