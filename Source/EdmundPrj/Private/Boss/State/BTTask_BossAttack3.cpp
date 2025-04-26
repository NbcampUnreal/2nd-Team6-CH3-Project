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
#include "TimerManager.h"

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
        return EBTNodeResult::Failed;
    }

    ABossAIController* BossController = Cast<ABossAIController>(AIController);
    if (!BossController)
    {
        return EBTNodeResult::Failed;
    }
    BossController->CurrentAttackTask = this;

    BossRef = Cast<ABoss>(AIController->GetPawn());
    if (!BossRef)
    {
        return EBTNodeResult::Failed;
    }

    DashFrequency = BossRef->BossDashFrequency;
    DashDamping = BossRef->BossDashDamping;

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

void UBTTask_BossAttack3::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    if (!BossRef) return;
    int32 Phase = BossRef->GetComboPhase();
    if (Phase == 1 || Phase == 2)
    {
        FVector Dir = (Cast<ABossAIController>(BossRef->GetController())->GetPlayerLocation()
            - BossRef->GetActorLocation()).GetSafeNormal();
        FRotator Curr = BossRef->GetActorRotation();
        FRotator Target = Dir.Rotation();
        float InterpSpeed = 360.0f;
        FRotator NewRot = FMath::RInterpTo(Curr, Target, DeltaSeconds, InterpSpeed);
        BossRef->SetActorRotation(NewRot);
    }
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

    if (BossRef->GameState)
    {
        BossRef->GameState->PlayMonsterSound(BossRef->CurrentAudioComp, BossRef->GetMonsterType(), ESoundType::Attack3);
    }

    ExecuteMeleeAttack();
    BossRef->SetComboPhase(2);
    ComboPhase = 2;
}



void UBTTask_BossAttack3::OnAttack2Notify()
{
    if (!BossRef)
    {
        return;
    }


    if (ComboPhase != 2)
    {
        return;
    }

    if (BossRef->GameState)
    {
        BossRef->GameState->PlayMonsterSound(BossRef->CurrentAudioComp, BossRef->GetMonsterType(), ESoundType::Attack3);
    }
    ExecuteMeleeAttack();
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

    int32 Phase = BossRef->GetComboPhase();
    float DashDistance = 0.f;
    float DashSpeed = 0.f;

    switch (Phase)
    {
    case 1:
        DashDistance = BossRef->MeleeAttackDashDistance_Attack1;
        DashSpeed = BossRef->MeleeAttackDashSpeed_Attack1;
        break;
    case 2:
        DashDistance = BossRef->MeleeAttackDashDistance_Attack2;
        DashSpeed = BossRef->MeleeAttackDashSpeed_Attack2;
        break;
    default:
        return;
    }

    FVector Dir = BossRef->GetActorForwardVector();
    if (auto AICon = Cast<ABossAIController>(BossRef->GetController()))
    {
        Dir = (AICon->GetPlayerLocation() - BossRef->GetActorLocation()).GetSafeNormal();
    }

    if (auto Anim = BossRef->GetMesh()->GetAnimInstance())
    {
        Anim->RootMotionMode = ERootMotionMode::IgnoreRootMotion;
    }

    UCharacterMovementComponent* MC = BossRef->GetCharacterMovement();
    if (MC)
    {
        MC->StopMovementImmediately();
        MC->SetMovementMode(MOVE_Flying);
        MC->GroundFriction = 0.f;
        MC->BrakingFriction = 0.f;
        MC->BrakingFrictionFactor = 0.f;
    }

    if (MC)
    {
        MC->AddImpulse(Dir * DashSpeed, true);
    }

    TWeakObjectPtr<ABoss> WeakBoss = BossRef;
    float DashTime = 1.f;
    FTimerHandle TimerHandle;
    BossRef->GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        FTimerDelegate::CreateLambda([WeakBoss]()
            {
                if (auto Boss = WeakBoss.Get())
                {
                    if (auto M = Boss->GetCharacterMovement())
                    {
                        M->StopMovementImmediately();
                        M->SetMovementMode(MOVE_Walking);
                        M->GravityScale = 1.f;
                        M->GroundFriction = 8.f;
                        M->BrakingFriction = 8.f;
                        M->BrakingFrictionFactor = 2.f;
                    }
                }
            }),
        DashTime,
        false
    );
}

void UBTTask_BossAttack3::OnDashFinished()
{
    if (!BossRef)
        return;

    if (auto MoveComp = BossRef->GetCharacterMovement())
    {
        MoveComp->SetMovementMode(MOVE_Walking);
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
        ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
        if (PlayerCharacter)
        {
            FVector KnockbackDirection = (PlayerCharacter->GetActorLocation() - BossRef->GetActorLocation()).GetSafeNormal();
            KnockbackDirection.Z = 0.0f;
            float KnockbackForce = 1500.0f;
            PlayerCharacter->LaunchCharacter(KnockbackDirection * KnockbackForce, true, false);
        }

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

    if (BossRef->GameState)
    {
        BossRef->GameState->PlayMonsterSound(BossRef->CurrentAudioComp, BossRef->GetMonsterType(), ESoundType::Attack);
    }

    ABoss_Attack1_Bullet* Bullet = ABoss_Attack1_Bullet::GetBulletFromPool(BossRef->GetWorld(), BossRef->Attack1BulletClass);
    if (Bullet)
    {
        Bullet->FireProjectile(NewSpawnLocation, NewRotation, BulletDirection);
    }

    FiredBulletCount++;
}