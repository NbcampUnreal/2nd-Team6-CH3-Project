#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

TArray<ABoss_Attack1_Bullet*> ABoss_Attack1_Bullet::BulletPool;

ABoss_Attack1_Bullet::ABoss_Attack1_Bullet()
{
    PrimaryActorTick.bCanEverTick = true;
    ShpereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ShpereComp"));
    ShpereComp->SetCollisionProfileName(TEXT("BlockAll"));
    ShpereComp->OnComponentHit.AddDynamic(this, &ABoss_Attack1_Bullet::OnHit);
    RootComponent = ShpereComp;

    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    BulletMesh->SetupAttachment(RootComponent);
    BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    SetActorHiddenInGame(false);
    SetActorEnableCollision(false);
}

void ABoss_Attack1_Bullet::BeginPlay()
{
    Super::BeginPlay();
    if (BulletPool.Num() == 0)
    {
        BulletPool.Empty();
    }

    BulletPool.Add(this);
}

void ABoss_Attack1_Bullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        FVector NewLocation = GetActorLocation() + GetActorForwardVector() * BulletSpeed * DeltaTime;
        SetActorLocation(NewLocation);
        TraveledDistance += BulletSpeed * DeltaTime;

        UE_LOG(LogTemp, Log, TEXT("Bullet moving: %s"), *NewLocation.ToString());

        if (TraveledDistance >= MaxDistance)
        {
            UE_LOG(LogTemp, Log, TEXT("Bullet reached max distance"));
            SetActorHiddenInGame(true);
            SetActorEnableCollision(false);
            bIsActive = false;
        }
    }
}


void ABoss_Attack1_Bullet::FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{
    if (!SpawnLocation.IsNearlyZero())
    {
        SetActorLocation(SpawnLocation);
        SetActorRotation(SpawnRotation);
        UE_LOG(LogTemp, Log, TEXT("Bullet spawned at: %s"), *SpawnLocation.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("FireProjectile: Invalid SpawnLocation"));
    }

    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    bIsActive = true;
    TraveledDistance = 0.0f;

    UE_LOG(LogTemp, Log, TEXT("Bullet is now active at: %s"), *GetActorLocation().ToString());
}



void ABoss_Attack1_Bullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
        bIsActive = false;
    }

    BulletPool.Remove(this);
    UE_LOG(LogTemp, Log, TEXT("Bullet fire - pool"));
}

ABoss_Attack1_Bullet* ABoss_Attack1_Bullet::GetBulletFromPool(UWorld* World, TSubclassOf<ABoss_Attack1_Bullet> BullectClass)
{
    for (ABoss_Attack1_Bullet* Bullet : BulletPool)
    {
        if (!Bullet->IsActive())
        {
            UE_LOG(LogTemp, Log, TEXT("Reusing bullet from pool at location: %s"), *Bullet->GetActorLocation().ToString());
            return Bullet;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("No available bullet, spawning new one"));

    if (World && BullectClass)
    {
        ABoss_Attack1_Bullet* NewBullet = World->SpawnActor<ABoss_Attack1_Bullet>(BullectClass);
        if (NewBullet)
        {
            NewBullet->SetActorHiddenInGame(true);
            NewBullet->SetActorEnableCollision(false);
            BulletPool.Add(NewBullet);
            return NewBullet;
        }
    }
    return nullptr;
}


void ABoss_Attack1_Bullet::ActivateBullet()
{
    bIsActive = true;
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    TraveledDistance = 0.0f;

    if (BulletMesh)
    {
        BulletMesh->SetVisibility(true);
    }
   
}