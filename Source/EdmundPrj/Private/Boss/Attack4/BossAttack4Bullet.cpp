#include "Boss/Attack4/BossAttack4Bullet.h"
#include "Kismet/GameplayStatics.h"

ABossAttack4Bullet::ABossAttack4Bullet()
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
    BulletMesh->OnComponentHit.AddDynamic(this, &ABossAttack4Bullet::OnHit);
}

void ABossAttack4Bullet::BeginPlay()
{
    Super::BeginPlay();

    if (!BulletMesh->OnComponentHit.IsAlreadyBound(this, &ABossAttack4Bullet::OnHit))
    {
        BulletMesh->OnComponentHit.AddDynamic(this, &ABossAttack4Bullet::OnHit);
    }

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(TrackingTimerHandle, this, &ABossAttack4Bullet::MoveTowardsPlayer, 0.05f, true);
        GetWorld()->GetTimerManager().SetTimer(LifeSpanHandle, this, &ABossAttack4Bullet::DestroyBullet, 5.0f, false);
    }
}

void ABossAttack4Bullet::MoveTowardsPlayer()
{
    AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player)
    {
        GetWorld()->GetTimerManager().ClearTimer(TrackingTimerHandle);
        return;
    }

    FVector Direction = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FVector NewLocation = GetActorLocation() + Direction * Speed * 0.05f;
    SetActorLocation(NewLocation);
}

void ABossAttack4Bullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

    if (OtherActor)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            UE_LOG(LogTemp, Warning, TEXT("BossAttack4Bullet - Hit - Destroy Bullet"));
            Destroy();
        }
    }
}

void ABossAttack4Bullet::DestroyBullet()
{
    GetWorld()->GetTimerManager().ClearTimer(TrackingTimerHandle);
    Destroy();
}
