#include "Boss/State/BTTask_BossInitialize.h"
#include "Boss/Boss.h"
#include "Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"

UBTTask_BossInitialize::UBTTask_BossInitialize()
{
    NodeName = TEXT("Boss Initialize");
}

EBTNodeResult::Type UBTTask_BossInitialize::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    OwnerCompRef = &OwnerComp;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    BossCharacter = Cast<ABoss>(AIController->GetPawn());
    if (!BossCharacter)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    int32 bIsStart = BlackboardComp->GetValueAsInt(TEXT("bIsStart"));
    if (bIsStart != 1)
    {
        return EBTNodeResult::Succeeded;
    }

    BossCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

    UAnimInstance* AnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        UBoss_AnimInstance* BossAnimInstance = Cast<UBoss_AnimInstance>(AnimInstance);
        if (BossAnimInstance && BossAnimInstance->Initialize)
        {
            FOnMontageEnded MontageEndDelegate;
            MontageEndDelegate.BindUObject(this, &UBTTask_BossInitialize::OnMontageEnded);
            AnimInstance->Montage_Play(BossAnimInstance->Initialize);
            AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, BossAnimInstance->Initialize);
        }
    }

    return EBTNodeResult::InProgress;
}


void UBTTask_BossInitialize::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (!BossCharacter)
    {
        return;
    }

    AAIController* AIController = Cast<AAIController>(BossCharacter->GetController());
    if (AIController)
    {
        UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsInt(TEXT("bIsStart"), 0);
        }
    }

    BossCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

    if (OwnerCompRef)
    {
        FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded);
    }
}
