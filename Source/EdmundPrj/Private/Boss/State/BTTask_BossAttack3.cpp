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
    bNotifyTick = false; // 애니메이션 노티파이에 의존하므로 Tick은 사용하지 않습니다.
    BossRef = nullptr;
    CachedOwnerComp = nullptr;
    ComboPhase = 0;
}

// ExecuteTask에서 블랙보드 대신 클래스 멤버 변수 사용
EBTNodeResult::Type UBTTask_BossAttack3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AIController와 Boss 확인
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
    ComboPhase = 0; // 콤보 초기화

    UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
    if (!BBComp)
    {
        return EBTNodeResult::Failed;
    }
    int32 NextAttackValue = BBComp->GetValueAsInt("NextAttack");

    // 공격 타입에 맞게 공격 몽타주 재생
    if (NextAttackValue == 3)
    {
        PlayAttack3Montage();

        // 콤보 단계에 맞게 공격 진행
        if (ComboPhase == 0)
        {
            OnAttack1Notify();
        }
        else if (ComboPhase == 1)
        {
            OnAttack2Notify();
        }
        else
        {
            FinishComboAttack();
        }
    }

    return EBTNodeResult::InProgress;
}


// OnAttack1Notify, OnAttack2Notify, FinishComboAttack에서 블랙보드 사용 없이 ComboPhase 직접 업데이트
void UBTTask_BossAttack3::OnAttack1Notify()
{
    // 콤보 1단계로 변경
    ComboPhase = 1;
    ExecuteMeleeAttack(); // 돌진 처리
}


void UBTTask_BossAttack3::OnAttack2Notify()
{
    // 콤보 2단계로 변경
    ComboPhase = 2;
    ExecuteMeleeAttack(); // 돌진 처리
}

void UBTTask_BossAttack3::FinishComboAttack()
{
    if (!BossRef)
    {
        return;
    }

    // 공격 쿨타임 갱신
    BossRef->UpdateAttackCooldown(3);
    BossRef->CurrentAttackTask = nullptr;

    // 콤보가 끝나면 ComboPhase 초기화
    ComboPhase = 0;

    // Behavior Tree에서 현재 태스크를 성공적으로 종료
    if (CachedOwnerComp)
    {
        BossRef->SetbChaseComplete(true);
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
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

    // ComboPhase 0: 1타, ComboPhase 1: 2타
    if (ComboPhase == 0)
    {
        DashDistance = BossRef->MeleeAttackDashDistance_Attack1;
        DashSpeed = BossRef->MeleeAttackDashSpeed_Attack1;
    }
    else if (ComboPhase == 1)
    {
        DashDistance = BossRef->MeleeAttackDashDistance_Attack2;
        DashSpeed = BossRef->MeleeAttackDashSpeed_Attack2;
    }

    // 돌진 방향은 보스의 정면 방향
    FVector DashDirection = BossRef->GetActorForwardVector();

    // 돌진 효과: LaunchCharacter를 사용하여 지정된 속도로 돌진
    FVector LaunchVelocity = DashDirection * DashSpeed;
    BossRef->LaunchCharacter(LaunchVelocity, true, true);

    // 추가: 이동 거리를 고려한 위치 변경 (필요 시)
    // FVector NewLocation = BossRef->GetActorLocation() + DashDirection * DashDistance;
    // BossRef->SetActorLocation(NewLocation, true);

    BossRef->MonsterAttackCheck();
}
