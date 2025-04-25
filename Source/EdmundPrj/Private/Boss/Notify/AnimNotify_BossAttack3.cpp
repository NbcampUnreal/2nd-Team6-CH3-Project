#include "Boss/Notify/AnimNotify_BossAttack3.h"
#include "Boss/State/BTTask_BossAttack3.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Boss/Boss.h"

void UAnimNotify_BossAttack3::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp)
    {
        return;
    }

    AActor* OwnerActor = MeshComp->GetOwner();
    if (!OwnerActor)
    {
        return;
    }

    ABoss* BossRef = Cast<ABoss>(OwnerActor);
    if (!BossRef)
    {
        return;
    }

    if (NotifyName == FName("BossAttack3_1"))
    {
        BossRef->SetComboPhase(1);
        BossRef->CurrentAttackTask->OnAttack1Notify();
    }
    else if (NotifyName == FName("BossAttack3_2"))
    {
        BossRef->SetComboPhase(2);
        BossRef->CurrentAttackTask->OnAttack2Notify();
    }
    else if (NotifyName == FName("BossAttack3_3"))
    {
        BossRef->SetComboPhase(3);
        BossRef->CurrentAttackTask->FinishComboAttack();
    }
}


