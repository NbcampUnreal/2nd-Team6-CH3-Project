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

    UE_LOG(LogTemp, Log, TEXT("EnterState: Player found at %s"), *Player->GetActorLocation().ToString());

    FireBullet();
}


void UBoss_Attack1::ExitState()
{
	BossRef = nullptr;
	UE_LOG(LogTemp, Log, TEXT("Boss is exiting ATTACK1"));
}


void UBoss_Attack1::FireBullet()
{
    UE_LOG(LogTemp, Log, TEXT("FireBullet 호출됨"));

    if (!BossRef || !BossRef->GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("FireBullet: BossRef or World is NULL"));
        return;
    }

    if (!BossRef->MuzzleLocation)
    {
        UE_LOG(LogTemp, Error, TEXT("FireBullet: MuzzleLocation is NULL"));
        return;
    }

    if (!BossRef->Attack1BulletClass)
    {
        UE_LOG(LogTemp, Error, TEXT("FireBullet: Attack1BulletClass is NULL"));
        return;
    }

    FVector SpawnLocation = BossRef->MuzzleLocation->GetComponentLocation();
    FRotator SpawnRotation = BossRef->MuzzleLocation->GetComponentRotation();

    UE_LOG(LogTemp, Log, TEXT("FireBullet: Bullet SpawnLocation: %s"), *SpawnLocation.ToString());

    ABoss_Attack1_Bullet* Bullet = ABoss_Attack1_Bullet::GetBulletFromPool(BossRef->GetWorld(), BossRef->Attack1BulletClass);

    if (Bullet)
    {
        Bullet->SetActorLocation(SpawnLocation);
        Bullet->SetActorRotation(SpawnRotation);
        Bullet->SetActorHiddenInGame(false);
        Bullet->SetActorEnableCollision(true);
        Bullet->ActivateBullet();
        UE_LOG(LogTemp, Log, TEXT("FireBullet: Bullet fire success at %s"), *Bullet->GetActorLocation().ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("FireBullet: Bullet fire fail"));
    }
}


