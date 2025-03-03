#include "Boss/State/BTTask_BossInitialize.h"
#include "Boss/Boss.h"
#include "Boss/BossAIController.h"
#include "Boss/Boss_AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_BossInitialize::UBTTask_BossInitialize()
{
    NodeName = TEXT("Boss Initialize");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BossInitialize::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    OwnerCompRef = &OwnerComp;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTTask_BossInitialize::ExecuteTask - AI Controller is null"));
        return EBTNodeResult::Failed;
    }

    BossCharacter = Cast<ABoss>(AIController->GetPawn());
    if (!BossCharacter || !BossCharacter->GetCharacterMovement())
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTTask_BossInitialize::ExecuteTask - BossCharacter is null or has no movement component"));
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (!BlackboardComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBTTask_BossInitialize::ExecuteTask - Blackboard Component is null"));
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
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT("Boss is descending..."));
            }
            FVector NewLocation = BossCharacter->GetActorLocation();
            NewLocation.Z -= 200.0f * DeltaSeconds;
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
