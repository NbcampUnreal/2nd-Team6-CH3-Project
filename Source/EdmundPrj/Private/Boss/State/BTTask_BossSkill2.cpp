#include "Boss/State/BTTask_BossSkill2.h"
#include "Boss/Boss.h"
#include "Boss/Boss_AnimInstance.h"
#include "AIController.h"
#include "Engine/World.h"
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

    BossRef = Cast<ABoss>(AICon->GetPawn());
    if (!BossRef)
    {
        return EBTNodeResult::Failed;
    }

    if (BossRef->IsSkill2Invulnerable())
    {
        return EBTNodeResult::Succeeded;
    }

    PlaySkill2Animation();

    if (BossRef && BossRef->Skill2NewMaterial)
    {
        BossRef->SetbSkill2Used(true);
        BossRef->SetSkill2Invulnerable(true);
        BossRef->SetbIsInvulnerable(true);
        BossRef->GetMesh()->SetMaterial(0, BossRef->Skill2NewMaterial);

        if (BossRef->Skill2ShieldNiagara)
        {
            BossRef->Skill2ShieldNiagara->Activate(true);
        }
    }
    return EBTNodeResult::Succeeded;
}


void UBTTask_BossSkill2::PlaySkill2Animation()
{
    if (!BossRef) return;

    USkeletalMeshComponent* Mesh = BossRef->GetMesh();
    if (!Mesh) return;

    UAnimInstance* AnimInst = Mesh->GetAnimInstance();
    if (!AnimInst) return;

    UBoss_AnimInstance* BossAnimInst = Cast<UBoss_AnimInstance>(AnimInst);
    if (BossAnimInst && BossAnimInst->Skill2Montage)
    {
        if (AnimInst->Montage_IsPlaying(BossAnimInst->Skill2Montage))
        {
            AnimInst->Montage_Stop(0.1f, BossAnimInst->Skill2Montage);
        }
        AnimInst->Montage_Play(BossAnimInst->Skill2Montage);
    }
}
