#include "Boss/Boss_AnimInstance.h"
#include "Boss/Boss.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBoss_AnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    APawn* OwningPawn = TryGetPawnOwner();
    if (!OwningPawn)
    {
        //UE_LOG(LogTemp, Warning, TEXT("[UBoss_AnimInstance] TryGetPawnOwner() Returned NULL!"));
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UBoss_AnimInstance::InitializeBossRef);
        return; 
    }

    //UE_LOG(LogTemp, Log, TEXT("[UBoss_AnimInstance] TryGetPawnOwner() succeeded: %s"), *OwningPawn->GetName());

    BossRef = Cast<ABoss>(OwningPawn);
    if (!BossRef)
    {
        //UE_LOG(LogTemp, Error, TEXT("[UBoss_AnimInstance] BossRef is NULL after casting! Check if the Pawn is correctly set as ABoss."));
        return;
    }

    MovementComponent = BossRef->GetCharacterMovement();
    if (!MovementComponent)
    {
        //UE_LOG(LogTemp, Error, TEXT("[UBoss_AnimInstance] MovementComponent is NULL! Check if ABoss properly initializes UCharacterMovementComponent."));
        return;
    }

    bIsFalling = MovementComponent->IsFalling();

    //UE_LOG(LogTemp, Log, TEXT("[UBoss_AnimInstance] Initialization Complete! BossRef: %s"), *BossRef->GetName());
}


void UBoss_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (MovementComponent)
    {
        float CurrentSpeed = MovementComponent->Velocity.Size();

        MonsterMoveSpeed = CurrentSpeed;
        bIsMoving = CurrentSpeed > 0.0f;
    }

    if (BossRef)
    {
        Attack1Count = BossRef->Attack1Count;
        bIsFalling = BossRef->GetCharacterMovement()->IsFalling();
        Fly = bIsFalling ? 1.0f : 0.0f;
    }
}

void UBoss_AnimInstance::AnimNotify_AttackCheck()
{
    //UE_LOG(LogTemp, Log, TEXT("AttackCheck AnimNotify triggered."));
}

void UBoss_AnimInstance::InitializeBossRef()
{
    APawn* OwningPawn = TryGetPawnOwner();
    if (!OwningPawn)
    {
        //UE_LOG(LogTemp, Warning, TEXT("[UBoss_AnimInstance] Delayed TryGetPawnOwner() Returned NULL!"));
        return;
    }

    //UE_LOG(LogTemp, Log, TEXT("[UBoss_AnimInstance] TryGetPawnOwner() succeeded: %s"), *OwningPawn->GetName());

    BossRef = Cast<ABoss>(OwningPawn);
    if (!BossRef)
    {
        //UE_LOG(LogTemp, Error, TEXT("[UBoss_AnimInstance] BossRef is NULL after casting!"));
        return;
    }
}