#include "Boss/Boss_AnimInstance.h"
#include "Boss/Boss.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBoss_AnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    if (APawn* OwningPawn = TryGetPawnOwner())
    {
        if (!OwningPawn)
        {
            UE_LOG(LogTemp, Warning, TEXT("TryGetPawnOwner() Returned NULL!"));
            return;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("TryGetPawnOwner() succeeded: %s"), *OwningPawn->GetName());
            BossRef = Cast<ABoss>(OwningPawn);
            if (BossRef)
            {
                MovementComponent = BossRef->GetCharacterMovement();
            }
        }
    }
}



void UBoss_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (MovementComponent)
    {
        float CurrentSpeed = MovementComponent->Velocity.Size();

        // 보스의 MonsterMoveSpeed를 업데이트하는 대신,
        // 애님 인스턴스 자신의 MonsterMoveSpeed를 업데이트
        MonsterMoveSpeed = CurrentSpeed;
    }

    if (BossRef)
    {
        Attack1Count = BossRef->Attack1Count;
    }
}


void UBoss_AnimInstance::AnimNotify_Attack1()
{
    if (BossRef)
    {
        BossRef->SetAttack1Count(0);
    }
    Attack1Count = 0;
}

void UBoss_AnimInstance::AnimNotify_AttackCheck()
{
    UE_LOG(LogTemp, Log, TEXT("AttackCheck AnimNotify triggered."));
    // 실제 공격 판정 로직 실행, 예: BossRef->ExecuteMeleeAttackCheck();
}
