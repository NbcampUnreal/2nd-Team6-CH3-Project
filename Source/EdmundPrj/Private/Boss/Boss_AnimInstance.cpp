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
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UBoss_AnimInstance::InitializeBossRef);
        return; 
    }

    BossRef = Cast<ABoss>(OwningPawn);
    if (!BossRef)
    {
        return;
    }

    MovementComponent = BossRef->GetCharacterMovement();
    if (!MovementComponent)
    {
        return;
    }

    bIsFalling = MovementComponent->IsFalling();
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

void UBoss_AnimInstance::InitializeBossRef()
{
    APawn* OwningPawn = TryGetPawnOwner();
    if (!OwningPawn)
    {
        return;
    }

    BossRef = Cast<ABoss>(OwningPawn);
    if (!BossRef)
    {
        return;
    }
}