#include "Boss/State/BTTask_BossSkill2.h"
#include "Boss/Boss.h"
#include "Boss/Boss_AnimInstance.h"
#include "AIController.h"
#include "Engine/World.h"
#include "Boss/Boss_AnimInstance.h"
#include "TimerManager.h"

UBTTask_BossSkill2::UBTTask_BossSkill2()
{
    NodeName = TEXT("Boss Skill2");
    bNotifyTick = false;
    BossRef = nullptr;
}

EBTNodeResult::Type UBTTask_BossSkill2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        return EBTNodeResult::Failed;
    }

    ABoss* BossRefLocal = Cast<ABoss>(AICon->GetPawn());
    if (!BossRefLocal)
    {
        return EBTNodeResult::Failed;
    }
    BossRef = BossRefLocal;
    CachedOwnerComp = &OwnerComp;

    UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
    if (!AnimInst)
    {
        return EBTNodeResult::Failed;
    }

    AnimInst->bIsSkill2 = true;
    AnimInst->Skill2HintType = 0; 
    AnimInst->Skill2Iteration = 0; 

    float Skill2Duration = 5.0f; 

    BossRef->GetWorldTimerManager().SetTimer(
        TimerHandle,
        FTimerDelegate::CreateLambda([this, AnimInst]()
            {
                AnimInst->bIsSkill2 = false;
                if (CachedOwnerComp)
                {
                    FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
                }
            }),
        Skill2Duration,
        false
    );

    return EBTNodeResult::InProgress;
}
