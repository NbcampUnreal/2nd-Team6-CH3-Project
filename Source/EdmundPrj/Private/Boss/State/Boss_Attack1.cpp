#include "Boss/State/Boss_Attack1.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"

UBoss_Attack1::UBoss_Attack1()
{
}

void UBoss_Attack1::EnterState(ABoss* Boss)
{
    Super::EnterState(Boss);
    UE_LOG(LogTemp, Log, TEXT("Boss is ATTACK1"));

    if (!Boss) return;
    BossRef = Boss;

    AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
    if (!Player)
    {
        UE_LOG(LogTemp, Error, TEXT("EnterState: Player is NULL"));
        return;
    }

    FVector PlayerLocation = Player->GetActorLocation();
    FVector BossLocation = BossRef->GetActorLocation();
    FVector Direction = (PlayerLocation - BossLocation).GetSafeNormal();
    FRotator TargetRotation = Direction.Rotation();

    // 일정 시간 동안 보스 회전
    BossRef->GetWorld()->GetTimerManager().SetTimer(
        RotationTimerHandle,
        [this, TargetRotation]() {
            BossRef->SetActorRotation(TargetRotation);
            UE_LOG(LogTemp, Log, TEXT("Boss fully rotated towards %s"), *TargetRotation.ToString());
            FireBullet();
        },
        0.3f,  // 0.3초 후 실행 (회전 시간 확보)
        false
    );
}

void UBoss_Attack1::ExitState()
{
	BossRef = nullptr;
	UE_LOG(LogTemp, Log, TEXT("Boss is exiting ATTACK1"));
}


void UBoss_Attack1::FireBullet()
{
    if (!BossRef || !BossRef->GetWorld() || !BossRef->MuzzleLocation)
    {
        UE_LOG(LogTemp, Error, TEXT("FireBullet: BossRef, World, or MuzzleLocation is NULL"));
        return;
    }

    if (!BossRef->Attack1BulletClass)
    {
        UE_LOG(LogTemp, Error, TEXT("FireBullet: Attack1BulletClass is NULL"));
        return;
    }

    FVector SpawnLocation = BossRef->MuzzleLocation->GetComponentLocation();

    AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
    if (!Player)
    {
        UE_LOG(LogTemp, Error, TEXT("FireBullet: Player is NULL"));
        return;
    }

    FVector PlayerLocation = Player->GetActorLocation();
    FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();
    FRotator TargetRotation = Direction.Rotation();

    // 보스가 부드럽게 회전하도록 보간 적용
    FRotator NewRotation = FMath::RInterpTo(BossRef->GetActorRotation(), TargetRotation, BossRef->GetWorld()->GetDeltaSeconds(), 5.0f);
    BossRef->SetActorRotation(NewRotation);

    UE_LOG(LogTemp, Log, TEXT("FireBullet: Boss rotating smoothly towards %s"), *NewRotation.ToString());

    ABoss_Attack1_Bullet* Bullet = ABoss_Attack1_Bullet::GetBulletFromPool(BossRef->GetWorld(), BossRef->Attack1BulletClass);

    if (Bullet)
    {
        Bullet->SetActorLocation(SpawnLocation);
        Bullet->SetActorRotation(TargetRotation);
        Bullet->SetActorHiddenInGame(false);
        Bullet->SetActorEnableCollision(true);
        Bullet->ActivateBullet();

        UE_LOG(LogTemp, Log, TEXT("Bullet fired successfully at %s"), *Bullet->GetActorLocation().ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Bullet fire failed"));
    }
}



