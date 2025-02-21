#include "Boss/Boss_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

void UBoss_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* OwningPawn = TryGetPawnOwner();
    if (OwningPawn)
    {
        bIsMoving = OwningPawn->GetVelocity().Size() > 3.0f;
    }
}

