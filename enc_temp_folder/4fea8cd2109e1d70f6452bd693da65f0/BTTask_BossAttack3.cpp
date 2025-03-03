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

    // 매 발사마다 최신 노즐 위치와 회전값을 가져옴
    FRotator NewRotation = BossRef->MuzzleLocation->GetComponentRotation();
    FVector NewSpawnLocation = BossRef->MuzzleLocation->GetComponentLocation();

    // 플레이어 태그가 붙은 액터의 Head 소켓 위치로 높이 조정 시도
    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsWithTag(BossRef->GetWorld(), FName("Player"), PlayerActors);
    if (PlayerActors.Num() > 0)
    {
        // 첫 번째 플레이어 액터 선택
        AActor* PlayerActor = PlayerActors[0];
        // 플레이어의 스켈레탈 메시 컴포넌트 찾기
        USkeletalMeshComponent* SkeletalMesh = PlayerActor->FindComponentByClass<USkeletalMeshComponent>();
        if (SkeletalMesh && SkeletalMesh->DoesSocketExist(FName("Head")))
        {
            FVector HeadLocation = SkeletalMesh->GetSocketLocation(FName("Head"));
            // 기존 NewSpawnLocation의 Z값을 Head 소켓의 Z값으로 교체하여 높이 조정
            NewSpawnLocation.Z = HeadLocation.Z;
        }
    }
    // 플레이어 액터나 Head 소켓이 없으면 기존 NewSpawnLocation 사용

    FVector BulletDirection = NewRotation.Vector();

    ABoss_Attack1_Bullet* Bullet = ABoss_Attack1_Bullet::GetBulletFromPool(BossRef->GetWorld(), BossRef->Attack1BulletClass);
    if (Bullet)
    {
        Bullet->FireProjectile(NewSpawnLocation, NewRotation, BulletDirection);
    }

    FiredBulletCount++;
}

