#include "Boss/State/BTTask_BossAttack3.h"
#include "Boss/Boss.h"
#include "AIController.h"
#include "Engine/World.h"
#include "Behaviortree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "Boss/BossAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Boss/Boss_AnimInstance.h"
#include "Kismet/GameplayStatics.h"

UBTTask_BossAttack3::UBTTask_BossAttack3()
{
    NodeName = TEXT("Boss Attack3");
    bNotifyTick = false; 
    BossRef = nullptr;
    CachedOwnerComp = nullptr;
    ComboPhase = 0;
}

EBTNodeResult::Type UBTTask_BossAttack3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    CachedOwnerComp = &OwnerComp;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        //UE_LOG(LogTemp, Error, TEXT("Boss AIController is NULL"));
        return EBTNodeResult::Failed;
    }

    ABossAIController* BossController = Cast<ABossAIController>(AIController);
    if (!BossController)
    {
        //UE_LOG(LogTemp, Error, TEXT("Boss AIController cast failed"));
        return EBTNodeResult::Failed;
    }
    BossController->CurrentAttackTask = this;

    BossRef = Cast<ABoss>(AIController->GetPawn());
    if (!BossRef)
    {
        //UE_LOG(LogTemp, Error, TEXT("Boss Pawn is NULL"));
        return EBTNodeResult::Failed;
    }

    BossRef->SetComboPhase(1);
    ComboPhase = 1;
    //UE_LOG(LogTemp, Log, TEXT("ExecuteTask: ComboPhase 초기화 -> %d"), BossRef->GetComboPhase());

    PlayAttack3Montage();
    return EBTNodeResult::InProgress;
}

void UBTTask_BossAttack3::OnAttack1Notify()
{
    if (!BossRef)
    {
        //UE_LOG(LogTemp, Error, TEXT("OnAttack1Notify: BossRef is NULL!"));
        return;
    }

    //UE_LOG(LogTemp, Log, TEXT("OnAttack1Notify BEFORE: ComboPhase = %d"), BossRef->GetComboPhase());

    if (BossRef->GetComboPhase() != 1)
    {
        //UE_LOG(LogTemp, Error, TEXT("OnAttack1Notify: ComboPhase is not 1, something went wrong!"));
        return;
    }

    BossRef->SetComboPhase(2);
    ComboPhase = 2;

    //UE_LOG(LogTemp, Log, TEXT("OnAttack1Notify AFTER: ComboPhase = %d"), BossRef->GetComboPhase());

    ExecuteMeleeAttack();
}


void UBTTask_BossAttack3::OnAttack2Notify()
{
    if (!BossRef)
    {
        //UE_LOG(LogTemp, Error, TEXT("OnAttack2Notify: BossRef is NULL!"));
        return;
    }

    //UE_LOG(LogTemp, Log, TEXT("OnAttack2Notify BEFORE: ComboPhase = %d"), BossRef->GetComboPhase());

    if (ComboPhase != 2)
        return;

    BossRef->SetComboPhase(3);
    ComboPhase = 3;

    //UE_LOG(LogTemp, Log, TEXT("OnAttack2Notify AFTER: ComboPhase = %d"), BossRef->GetComboPhase());
}

void UBTTask_BossAttack3::OnAttack3Notify()
{
    if (!BossRef) return;

    Attack3_RangedAttackNotify();

    FinishComboAttack();
}


void UBTTask_BossAttack3::FinishComboAttack()
{
    if (!BossRef)
    {
        //UE_LOG(LogTemp, Error, TEXT("FinishComboAttack: BossRef is NULL!"));
        return;
    }

    //UE_LOG(LogTemp, Log, TEXT("FinishComboAttack: Executing, ComboPhase=%d"), BossRef->GetComboPhase());

    BossRef->UpdateAttackCooldown(3);
    BossRef->CurrentAttackTask = nullptr;

    ComboPhase = 0;
    BossRef->SetComboPhase(0);

    //UE_LOG(LogTemp, Log, TEXT("FinishComboAttack: ComboPhase 초기화 -> %d"), BossRef->GetComboPhase());

    if (CachedOwnerComp)
    {
        BossRef->SetbChaseComplete(true);
        //UE_LOG(LogTemp, Log, TEXT("FinishComboAttack: Behavior Tree Task Succeeded"));
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
    else
    {
        //UE_LOG(LogTemp, Error, TEXT("FinishComboAttack: CachedOwnerComp is NULL! Behavior Tree Task could not finish properly."));
    }
}

void UBTTask_BossAttack3::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}

void UBTTask_BossAttack3::PlayAttack3Montage()
{
    if (!BossRef)
    {
        return;
    }

    UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
    if (AnimInst && AnimInst->Attack3Montage)
    {
        BossRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimInst->Attack3Montage);
    }
}



void UBTTask_BossAttack3::ExecuteMeleeAttack()
{
    if (!BossRef)
    {
        //UE_LOG(LogTemp, Error, TEXT("ExecuteMeleeAttack: BossRef is NULL!"));
        return;
    }

    float DashDistance = 0.0f;
    float DashSpeed = 0.0f;

    int32 CurrentPhase = BossRef->GetComboPhase();
    //UE_LOG(LogTemp, Log, TEXT("ExecuteMeleeAttack: CurrentPhase = %d"), CurrentPhase);

    switch (CurrentPhase)
    {
    case 1:  // 1타 돌진
        DashDistance = BossRef->MeleeAttackDashDistance_Attack1;
        DashSpeed = BossRef->MeleeAttackDashSpeed_Attack1;
        break;
    case 2:  // 2타 돌진
        DashDistance = BossRef->MeleeAttackDashDistance_Attack2;
        DashSpeed = BossRef->MeleeAttackDashSpeed_Attack2;
        break;
    default:
        //UE_LOG(LogTemp, Warning, TEXT("ExecuteMeleeAttack: Unexpected ComboPhase = %d"), CurrentPhase);
        return;
    }

    //UE_LOG(LogTemp, Log, TEXT("ExecuteMeleeAttack: Using DashDistance = %f, DashSpeed = %f"), DashDistance, DashSpeed);

    FVector DashDirection = BossRef->GetActorForwardVector();
    FVector LaunchVelocity = DashDirection * DashSpeed;
    BossRef->LaunchCharacter(LaunchVelocity, true, true);

    BossRef->MonsterAttackCheck();
}

void UBTTask_BossAttack3::Attack3_ActivateMeleeCollision_Check1()
{
    if (!BossRef || !BossRef->Attack2_MeleeCollision)
        return;

    if (BossRef->GetWorld())
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(CollisionDisableTimerHandle_Check1);
        BossRef->GetWorld()->GetTimerManager().SetTimer(CollisionDisableTimerHandle_Check1, this, &UBTTask_BossAttack3::Attack3_DeactivateCollision_Check1, 0.5f, false);
    }

    if (!BossRef->Attack2_MeleeCollision->OnComponentBeginOverlap.IsAlreadyBound(this, &UBTTask_BossAttack3::OnMeleeCollisionOverlap_Check1))
    {
        BossRef->Attack2_MeleeCollision->OnComponentBeginOverlap.AddDynamic(this, &UBTTask_BossAttack3::OnMeleeCollisionOverlap_Check1);
    }
    BossRef->Attack2_MeleeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UBTTask_BossAttack3::Attack3_DeactivateCollision_Check1()
{
    if (!BossRef)
        return;

    if (BossRef->GetWorld())
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(CollisionDisableTimerHandle_Check1);
    }

    if (BossRef->Attack2_MeleeCollision)
    {
        BossRef->Attack2_MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        BossRef->Attack2_MeleeCollision->OnComponentBeginOverlap.RemoveDynamic(this, &UBTTask_BossAttack3::OnMeleeCollisionOverlap_Check1);
    }
}

void UBTTask_BossAttack3::OnMeleeCollisionOverlap_Check1(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!BossRef)
        return;

    if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
    {
        if (BossRef->GetWorld())
        {
            BossRef->GetWorld()->GetTimerManager().ClearTimer(CollisionDisableTimerHandle_Check1);
        }
        UGameplayStatics::ApplyDamage(OtherActor, 10.0f, nullptr, BossRef, nullptr);
        Attack3_DeactivateCollision_Check1();
    }
}


void UBTTask_BossAttack3::Attack3_ActivateMeleeCollision_Check2()
{
    if (!BossRef || !BossRef->Attack2_MeleeCollision)
        return;

    if (BossRef->GetWorld())
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(CollisionDisableTimerHandle_Check2);
        BossRef->GetWorld()->GetTimerManager().SetTimer(CollisionDisableTimerHandle_Check2, this, &UBTTask_BossAttack3::Attack3_DeactivateCollision_Check2, 0.5f, false);
    }

    if (!BossRef->Attack2_MeleeCollision->OnComponentBeginOverlap.IsAlreadyBound(this, &UBTTask_BossAttack3::OnMeleeCollisionOverlap_Check2))
    {
        BossRef->Attack2_MeleeCollision->OnComponentBeginOverlap.AddDynamic(this, &UBTTask_BossAttack3::OnMeleeCollisionOverlap_Check2);
    }
    BossRef->Attack2_MeleeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UBTTask_BossAttack3::Attack3_DeactivateCollision_Check2()
{
    if (!BossRef)
        return;

    if (BossRef->GetWorld())
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(CollisionDisableTimerHandle_Check2);
    }

    if (BossRef->Attack2_MeleeCollision)
    {
        BossRef->Attack2_MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        BossRef->Attack2_MeleeCollision->OnComponentBeginOverlap.RemoveDynamic(this, &UBTTask_BossAttack3::OnMeleeCollisionOverlap_Check2);
    }
}

void UBTTask_BossAttack3::OnMeleeCollisionOverlap_Check2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!BossRef)
        return;

    if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
    {
        if (BossRef->GetWorld())
        {
            BossRef->GetWorld()->GetTimerManager().ClearTimer(CollisionDisableTimerHandle_Check2);
        }
        UGameplayStatics::ApplyDamage(OtherActor, 1000.0f, nullptr, BossRef, nullptr);
        Attack3_DeactivateCollision_Check2();
    }
}

void UBTTask_BossAttack3::Attack3_RangedAttackNotify()
{
    if (!BossRef)
        return;

    if (BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->StopMovementImmediately();
    }

    CurrentRotation = BossRef->GetActorRotation();

    if (!BossRef->GetWorld() || !BossRef->MuzzleLocation || !BossRef->Attack1BulletClass)
        return;

    SpawnLocation = BossRef->MuzzleLocation->GetComponentLocation();

    BulletFireCount = 0;

    BossRef->GetWorld()->GetTimerManager().SetTimer(BulletFireTimerHandle, this, &UBTTask_BossAttack3::FireSingleBullet, 1.0f, true);
}

void UBTTask_BossAttack3::FireSingleBullet()
{
    if (!BossRef)
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(BulletFireTimerHandle);
        return;
    }

    if (BulletFireCount >= 10)
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(BulletFireTimerHandle);
        return;
    }

    float RandomYawOffset = FMath::RandRange(-30.0f, 30.0f);
    FRotator BulletRotation = CurrentRotation;
    BulletRotation.Yaw += RandomYawOffset;
    FVector BulletDirection = BulletRotation.Vector();

    ABoss_Attack1_Bullet* Bullet = ABoss_Attack1_Bullet::GetBulletFromPool(BossRef->GetWorld(), BossRef->Attack1BulletClass);
    if (Bullet)
    {
        Bullet->FireProjectile(SpawnLocation, BulletRotation, BulletDirection);
    }

    BulletFireCount++;
}