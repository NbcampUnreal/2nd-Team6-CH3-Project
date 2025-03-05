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
    bNotifyTick = true; 
    BossRef = nullptr;
    CachedOwnerComp = nullptr;
    ComboPhase = 0;

    bIsDashing = false;
    DashFrequency = 0;
    DashDamping = 0;
    DashCurrentVelocity = FVector::ZeroVector;
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

    DashFrequency = BossRef->BossDashFrequency;
    DashDamping = BossRef->BossDashFrequency;

    if (BossRef->GetWorld()->GetTimerManager().IsTimerActive(BulletFireTimerHandle))
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(BulletFireTimerHandle);
    }
    FiredBulletCount = 0;

    BossRef->SetComboPhase(1);
    ComboPhase = 1;

    PlayAttack3Montage();
    ExecuteMeleeAttack();
    return EBTNodeResult::InProgress;
}

void UBTTask_BossAttack3::OnAttack1Notify()
{
    if (!BossRef)
    {
        return;
    }

    if (BossRef->GetComboPhase() != 1)
    {
        return;
    }

    ExecuteMeleeAttack();
    BossRef->SetComboPhase(2);
    ComboPhase = 2;
    UE_LOG(LogTemp, Log, TEXT("OnAttack1Notify Called, Phase: %d"), ComboPhase);
}



void UBTTask_BossAttack3::OnAttack2Notify()
{
    if (!BossRef)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("OnAttack2Notify Called, Current ComboPhase: %d"), ComboPhase);

    if (ComboPhase != 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Phase in OnAttack2Notify: %d"), ComboPhase);
        return;
    }

    BossRef->SetComboPhase(3);
    ComboPhase = 3;
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
        return;
    }

    BossRef->UpdateAttackCooldown(3);
    BossRef->CurrentAttackTask = nullptr;

    ComboPhase = 0;
    BossRef->SetComboPhase(0);

    if (BossRef->GetWorld()->GetTimerManager().IsTimerActive(BulletFireTimerHandle))
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(BulletFireTimerHandle);
    }
    FiredBulletCount = 0;

    if (CachedOwnerComp)
    {
        BossRef->SetbChaseComplete(true);
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}


void UBTTask_BossAttack3::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (bIsDashing && BossRef)
    {
        FVector CurrentLocation = BossRef->GetActorLocation();

        int32 CurrentPhase = BossRef->GetComboPhase();
        float Damping = (CurrentPhase == 3) ? 3.0f : BossRef->BossDashDamping;

        float Frequency = DashFrequency;
        FVector Displacement = DashTargetLocation - CurrentLocation;
        FVector Acceleration = Frequency * Frequency * Displacement - 2.0f * Damping * Frequency * DashCurrentVelocity;

        DashCurrentVelocity += Acceleration * DeltaSeconds;
        FVector NewLocation = CurrentLocation + DashCurrentVelocity * DeltaSeconds;

        BossRef->SetActorLocation(NewLocation, true);

        if (FVector::DistSquared(NewLocation, DashTargetLocation) < FMath::Square(10.0f))
        {
            BossRef->SetActorLocation(DashTargetLocation, true);
            bIsDashing = false;

            if (CurrentPhase == 3)
            {
                DashCurrentVelocity = FVector::ZeroVector;
            }

            BossRef->MonsterAttackCheck();
        }
    }
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
        return;
    }

    float DashDistance = 0.0f;
    float DashSpeed = 0.0f;
    int32 CurrentPhase = BossRef->GetComboPhase();

    switch (CurrentPhase)
    {
    case 1:
        DashDistance = BossRef->MeleeAttackDashDistance_Attack1;
        DashSpeed = BossRef->MeleeAttackDashSpeed_Attack1;
        break;
    case 2:
        DashDistance = BossRef->MeleeAttackDashDistance_Attack2;
        DashSpeed = BossRef->MeleeAttackDashSpeed_Attack2;
        break;
    case 3:
        bIsDashing = false;
        DashCurrentVelocity = FVector::ZeroVector;
        if (BossRef->GetCharacterMovement())
        {
            BossRef->GetCharacterMovement()->StopMovementImmediately();
        }
        return;
    default:
        return;
    }

    FVector DirectionToTarget = BossRef->GetActorForwardVector();

    if (BossRef && BossRef->GetController() && CurrentPhase != 3)
    {
        ABossAIController* BossAIController = Cast<ABossAIController>(BossRef->GetController());
        if (BossAIController)
        {
            FVector TargetLocation = BossAIController->GetPlayerLocation();
            DirectionToTarget = (TargetLocation - BossRef->GetActorLocation()).GetSafeNormal();
        }
    }
    if (CurrentPhase != 3)
    {
        float RotationSpeed = 10.0f;
        FRotator TargetRotation = DirectionToTarget.Rotation();
        FRotator NewRotation = FMath::RInterpTo(BossRef->GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);
        BossRef->SetActorRotation(NewRotation);
    }

    bIsDashing = true;
    DashStartLocation = BossRef->GetActorLocation();
    DashTargetLocation = DashStartLocation + DirectionToTarget * DashDistance;
    DashCurrentVelocity = DirectionToTarget * DashSpeed;

    if (BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->StopMovementImmediately();
    }
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

        float DamageValue = BossRef->GetMonsterAttackDamage() * BossRef->GetAttack3Multiplier();

        UGameplayStatics::ApplyDamage(OtherActor, DamageValue, nullptr, BossRef, nullptr);
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

    BossRef->GetWorld()->GetTimerManager().ClearTimer(BulletFireTimerHandle);

    if (BossRef->GetCharacterMovement())
    {
        BossRef->GetCharacterMovement()->StopMovementImmediately();
    }

    CurrentRotation = BossRef->MuzzleLocation->GetComponentRotation();
    SpawnLocation = BossRef->MuzzleLocation->GetComponentLocation();

    AccumulatedDeltaTime = 0.0f;
    FiredBulletCount = 0;

    BossRef->GetWorld()->GetTimerManager().SetTimer(BulletFireTimerHandle, this, &UBTTask_BossAttack3::FireSingleBullet, BossRef->Attack3_3FireInterval, true);
}

FVector UBTTask_BossAttack3::GetAdjustedSpawnLocation(const FVector& Offset) const
{
    return SpawnLocation + Offset;
}

void UBTTask_BossAttack3::FireSingleBullet()
{
    if (!BossRef)
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(BulletFireTimerHandle);
        return;
    }

    if (FiredBulletCount >= BossRef->Attack3_3BulletNum)
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(BulletFireTimerHandle);
        return;
    }

    AccumulatedDeltaTime += BossRef->Attack3_3FireInterval;
    if (AccumulatedDeltaTime >= BossRef->Attack3_3FiringDuration)
    {
        BossRef->GetWorld()->GetTimerManager().ClearTimer(BulletFireTimerHandle);
        return;
    }

    FRotator NewRotation = BossRef->MuzzleLocation->GetComponentRotation();
    FVector NewSpawnLocation = BossRef->MuzzleLocation->GetComponentLocation();

    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsWithTag(BossRef->GetWorld(), FName("Player"), PlayerActors);
    if (PlayerActors.Num() > 0)
    {
        AActor* PlayerActor = PlayerActors[0];
        USkeletalMeshComponent* SkeletalMesh = PlayerActor->FindComponentByClass<USkeletalMeshComponent>();
        if (SkeletalMesh && SkeletalMesh->DoesSocketExist(FName("CharacterHead")))
        {
            FVector HeadLocation = SkeletalMesh->GetSocketLocation(FName("CharacterHead"));
            NewSpawnLocation.Z = HeadLocation.Z;
        }
        else
        {
            NewSpawnLocation.Z = 150.0f;
        }

        
    }

    FVector BulletDirection = NewRotation.Vector();

    ABoss_Attack1_Bullet* Bullet = ABoss_Attack1_Bullet::GetBulletFromPool(BossRef->GetWorld(), BossRef->Attack1BulletClass);
    if (Bullet)
    {
        Bullet->FireProjectile(NewSpawnLocation, NewRotation, BulletDirection);
    }

    FiredBulletCount++;
}



