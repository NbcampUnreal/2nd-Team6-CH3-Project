#include "Boss/State/BTTask_BossInitialize.h"
#include "Boss/Boss.h"
#include "Boss/BossAIController.h"
#include "Boss/Boss_AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimMontage.h"

UBTTask_BossInitialize::UBTTask_BossInitialize()
{
    NodeName = TEXT("Boss Initialize");
    bNotifyTick = true;
    bDescentSpeedCalculated = false;
    DescentSpeed = 0.f;
    TargetGroundLocation = FVector::ZeroVector;
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

    bDescentSpeedCalculated = false;
    DescentSpeed = 0.f;
    TargetGroundLocation = FVector::ZeroVector;

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

        if (CurrentSection == TEXT("BossDown"))
        {
            if (!bDescentSpeedCalculated)
            {
                FVector FootSocketLocation = BossCharacter->GetMesh()->GetSocketLocation(TEXT("Back_Left_FootCapsuleComponent"));
                FHitResult Hit;
                FVector TraceStart = FootSocketLocation;
                FVector TraceEnd = FootSocketLocation - FVector(0.f, 0.f, 5000.f);
                FCollisionQueryParams QueryParams;
                QueryParams.AddIgnoredActor(BossCharacter);

                if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
                {
                    TargetGroundLocation = Hit.Location;
                    float DistanceToGround = FootSocketLocation.Z - TargetGroundLocation.Z;

                    const FName TargetSectionName(TEXT("BossDown"));
                    int32 SectionIndex = CurrentMontage->GetSectionIndex(TargetSectionName);
                    if (SectionIndex != INDEX_NONE)
                    {
                        float SectionStartTime = 0.f;
                        float SectionEndTime = 0.f;
                        CurrentMontage->GetSectionStartAndEndTime(SectionIndex, SectionStartTime, SectionEndTime);
                        float CurrentTime = AnimInstance->Montage_GetPosition(CurrentMontage);
                        float RemainingTime = SectionEndTime - CurrentTime;
                        if (RemainingTime > 0.f)
                        {
                            DescentSpeed = DistanceToGround / RemainingTime;
                        }
                    }
                }
                bDescentSpeedCalculated = true;
            }

            FVector NewLocation = BossCharacter->GetActorLocation();
            NewLocation.Z -= DescentSpeed * DeltaSeconds;

            FVector NewFootSocketLocation = BossCharacter->GetMesh()->GetSocketLocation(TEXT("Back_Left_FootCapsuleComponent"));
            if (NewFootSocketLocation.Z <= TargetGroundLocation.Z)
            {
                float ActorSocketOffset = BossCharacter->GetActorLocation().Z - BossCharacter->GetMesh()->GetSocketLocation(TEXT("Back_Left_FootCapsuleComponent")).Z;
                float AdditionalDownOffset = 3000.f;
                NewLocation.Z = TargetGroundLocation.Z + ActorSocketOffset - AdditionalDownOffset;
            }

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
    FVector FootSocketLocation = BossCharacter->GetMesh()->GetSocketLocation(TEXT("Back_Left_FootCapsuleComponent"));
    FVector TraceStart = FootSocketLocation;
    FVector TraceEnd = FootSocketLocation - FVector(0.f, 0.f, 5000.f);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(BossCharacter);

    if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
    {
        float ActorSocketOffset = BossCharacter->GetActorLocation().Z - BossCharacter->GetMesh()->GetSocketLocation(TEXT("Back_Left_FootCapsuleComponent")).Z;
        FVector NewLocation = BossCharacter->GetActorLocation();
        float AdditionalDownOffset = 50.f;
        NewLocation.Z = Hit.Location.Z + ActorSocketOffset - AdditionalDownOffset;
        BossCharacter->SetActorLocation(NewLocation);
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

