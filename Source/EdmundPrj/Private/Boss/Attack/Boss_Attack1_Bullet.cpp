#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABoss_Attack1_Bullet::ABoss_Attack1_Bullet()
{
    PrimaryActorTick.bCanEverTick = true;

    // �浹 ������Ʈ �߰�
    ShpereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ShpereComp"));
    ShpereComp->SetCollisionProfileName(TEXT("BlockAll"));
    ShpereComp->OnComponentHit.AddDynamic(this, &ABoss_Attack1_Bullet::OnHit);
    RootComponent = ShpereComp;

    // �޽� �߰�
    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    BulletMesh->SetupAttachment(RootComponent);
    BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // ���ü� ���� (�߻� �� Ȱ��ȭ)
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

        // �ִ� �̵� �Ÿ� �ʰ� �� Ǯ������ ��ȯ
        if (TraveledDistance >= MaxDistance)
        {
            SetActorHiddenInGame(true);
            SetActorEnableCollision(false);
            bIsActive = false;
        }
    }
}

// Ǯ������ ȣ��� �� ���� 
void ABoss_Attack1_Bullet::FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{
    SetActorLocation(SpawnLocation);
    SetActorRotation(SpawnRotation);
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);

    bIsActive = true;
    TraveledDistance = 0.0f;
}

// �浹 �� Ǯ������ ��ȯ
void ABoss_Attack1_Bullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        // ��Ʈ ����Ʈ, ���� ���� �߰� ����
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
        bIsActive = false;
    }
}
