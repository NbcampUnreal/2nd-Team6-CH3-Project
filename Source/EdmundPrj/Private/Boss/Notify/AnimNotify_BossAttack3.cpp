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

    // 애니메이션 노티파이 값에 따라 ComboPhase를 업데이트
    if (NotifyName == FName("BossAttack3_1"))
    {
        BossRef->SetComboPhase(1);  // 1타 콤보
        UE_LOG(LogTemp, Log, TEXT("BossAttack3_1 $d"), BossRef->GetComboPhase());
        BossRef->CurrentAttackTask->OnAttack1Notify();
    }
    else if (NotifyName == FName("BossAttack3_2"))
    {
        BossRef->SetComboPhase(2);  // 2타 콤보
        UE_LOG(LogTemp, Log, TEXT("BossAttack3_2 - ComboPhase: %d"), BossRef->GetComboPhase());
        BossRef->CurrentAttackTask->OnAttack2Notify();
    }
    else if (NotifyName == FName("BossAttack3_3"))
    {
        BossRef->SetComboPhase(3);  // 콤보 종료
        UE_LOG(LogTemp, Log, TEXT("BossAttack3_3 - ComboPhase: %d"), BossRef->GetComboPhase());
        BossRef->CurrentAttackTask->FinishComboAttack();
    }
}


