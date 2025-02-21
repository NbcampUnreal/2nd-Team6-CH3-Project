#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABoss_Attack1_Bullet::ABoss_Attack1_Bullet()
{
    PrimaryActorTick.bCanEverTick = true;

    // 충돌 컴포넌트 추가
    ShpereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ShpereComp"));
    ShpereComp->SetCollisionProfileName(TEXT("BlockAll"));
    ShpereComp->OnComponentHit.AddDynamic(this, &ABoss_Attack1_Bullet::OnHit);
    RootComponent = ShpereComp;

    // 메시 추가
    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    BulletMesh->SetupAttachment(RootComponent);
    BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 가시성 숨김 (발사 시 활성화)
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
}

void ABoss_Attack1_Bullet::BeginPlay()
{
    Super::BeginPlay();
}

void ABoss_Attack1_Bullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        FVector NewLocation = GetActorLocation() + GetActorForwardVector() * BulletSpeed * DeltaTime;
        SetActorLocation(NewLocation);
        TraveledDistance += BulletSpeed * DeltaTime;

        // 최대 이동 거리 초과 시 풀링으로 반환
        if (TraveledDistance >= MaxDistance)
        {
            SetActorHiddenInGame(true);
            SetActorEnableCollision(false);
            bIsActive = false;
        }
    }
}

// 풀링에서 호출될 때 설정 
void ABoss_Attack1_Bullet::FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{
    SetActorLocation(SpawnLocation);
    SetActorRotation(SpawnRotation);
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);

    bIsActive = true;
    TraveledDistance = 0.0f;
}

// 충돌 시 풀링으로 반환
void ABoss_Attack1_Bullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        // 히트 이펙트, 사운드 등을 추가 가능
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
        bIsActive = false;
    }
}
