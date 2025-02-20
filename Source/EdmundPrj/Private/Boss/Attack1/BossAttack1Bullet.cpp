#include "Boss/Attack1/BossAttack1Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

ABossAttack1Bullet::ABossAttack1Bullet()
{
    PrimaryActorTick.bCanEverTick = false;

    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    RootComponent = BulletMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
    if (SphereMesh.Succeeded())
    {
        BulletMesh->SetStaticMesh(SphereMesh.Object);
        BulletMesh->SetWorldScale3D(FVector(0.5f));
    }

    BulletMesh->SetCollisionProfileName(TEXT("BlockAll"));
    BulletMesh->OnComponentHit.AddDynamic(this, &ABossAttack1Bullet::OnHit);
}

void ABossAttack1Bullet::BeginPlay()
{
    Super::BeginPlay();
}

void ABossAttack1Bullet::Initialize(FVector TargetLocation)
{
    FVector StartLocation = GetActorLocation();
    Direction = (TargetLocation - StartLocation).GetSafeNormal();

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
        GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
        GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &ABossAttack1Bullet::MoveBullet, 0.016f, true);
        GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ABossAttack1Bullet::OnDestroyTimerExpired, 3.0f, false);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BossAttack1Bullet - GetWorld() is nullptr! Timer not set."));
    }
}

void ABossAttack1Bullet::MoveBullet()
{
    if (!this || !IsValid(this))
    {
        UE_LOG(LogTemp, Error, TEXT("BossAttack1Bullet - Bullet is invalid! Stopping movement."));
        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
        return;
    }

    FVector NewLocation = GetActorLocation() + (Direction * Speed * 0.016f);
    SetActorLocation(NewLocation);
}

void ABossAttack1Bullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        Destroy();
    }
}

void ABossAttack1Bullet::OnDestroyTimerExpired()
{
    if (this && IsValid(this))
    {
        Destroy();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BossAttack1Bullet - Bullet is already destroyed or invalid!"));
    }
}
