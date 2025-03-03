#include "Boss/State/BTTask_BossInitialize.h"
#include "Boss/Boss.h"
#include "Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"

UBTTask_BossInitialize::UBTTask_BossInitialize()
{
    NodeName = TEXT("Boss Initialize");
    bNotifyTick = true;

EBTNodeResult::Type UBTTask_BossInitialize::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    OwnerCompRef = &OwnerComp;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    BossCharacter = Cast<ABoss>(AIController->GetPawn());
    if (!BossCharacter || !BossCharacter->GetCharacterMovement())
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    bool bIsStart = BlackboardComp->GetValueAsBool(TEXT("bIsStart"));
    if (!bIsStart)
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

void UBTTask_BossInitialize::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (!BossCharacter || !BossCharacter->GetMesh() || !BossCharacter->GetMesh()->GetAnimInstance())
    {
        return;
    }

    UAnimInstance* AnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
    UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();

    if (CurrentMontage)
    {
        FName CurrentSection = AnimInstance->Montage_GetCurrentSection(CurrentMontage);

        if (CurrentSection == "BossDown")
        {
            FVector SocketLocation = BossCharacter->GetMesh()->GetSocketLocation("Back_Right_FootCapsuleComponent");
            FHitResult Hit;
            FVector Start = SocketLocation;
            FVector End = Start - FVector(0, 0, 5000);

            FCollisionQueryParams QueryParams;
            QueryParams.AddIgnoredActor(BossCharacter);

            float DistanceToGround = 0.0f;
            float Offset = 50.0f;

            if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams))
            {
                DistanceToGround = Start.Z - Hit.Location.Z + Offset;
            }

            int32 SectionIndex = CurrentMontage->GetSectionIndex("BossDown");
            float SectionStartTime = 0.0f;
            float SectionLength = 0.0f;

            if (SectionIndex != INDEX_NONE)
            {
                const FCompositeSection& Section = CurrentMontage->GetAnimCompositeSection(SectionIndex);
                SectionStartTime = Section.GetTime();
                if (SectionIndex + 1 < CurrentMontage->CompositeSections.Num())
                {
                    float NextSectionStartTime = CurrentMontage->GetAnimCompositeSection(SectionIndex + 1).GetTime();
                    SectionLength = NextSectionStartTime - SectionStartTime;
                }
                else
                {
                    SectionLength = CurrentMontage->GetPlayLength() - SectionStartTime;
                }
            }
            float MontagePosition = AnimInstance->Montage_GetPosition(CurrentMontage);
            float RemainingTime = SectionStartTime + SectionLength - MontagePosition;
            float DescendSpeed = (RemainingTime > 0) ? (DistanceToGround / RemainingTime) : 0.0f;

            FVector NewLocation = BossCharacter->GetActorLocation();
            NewLocation.Z -= DescendSpeed * DeltaSeconds;
            BossCharacter->SetActorLocation(NewLocation);
        }
    }
}




void UBTTask_BossInitialize::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (!BossCharacter)
    {
        return;
    }
    FHitResult Hit;
    FVector Start = BossCharacter->GetActorLocation();
    FVector End = Start - FVector(0, 0, 5000);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(BossCharacter);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams))
    {
        BossCharacter->SetActorLocation(Hit.Location);
    }

    BossCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

    AAIController* AIController = Cast<AAIController>(BossCharacter->GetController());
    if (AIController)
    {
        UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsBool(TEXT("bIsStart"), false);
        }
    }

    if (OwnerCompRef)
    {
        FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded);
    }
}
