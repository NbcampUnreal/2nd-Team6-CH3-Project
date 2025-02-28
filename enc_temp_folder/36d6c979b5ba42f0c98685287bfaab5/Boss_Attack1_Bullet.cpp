#include "Boss/Attack/Boss_Attack1_Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/Boss.h"

// 
TArray<ABoss_Attack1_Bullet*> ABoss_Attack1_Bullet::BulletPool;

ABoss_Attack1_Bullet::ABoss_Attack1_Bullet()
{
    PrimaryActorTick.bCanEverTick = true;

    ACollision = CreateDefaultSubobject<USphereComponent>(TEXT("ACollision"));
    ACollision->InitSphereRadius(10.0f);
    ACollision->SetCollisionProfileName(TEXT("BlockAll"));
    ACollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    ACollision->OnComponentHit.AddDynamic(this, &ABoss_Attack1_Bullet::OnHit);
    RootComponent = ACollision;

    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    BulletMesh->SetupAttachment(RootComponent);
    BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    BCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BCollision"));
    BCollision->SetupAttachment(RootComponent);
    BCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEffect"));
    ExplosionEffect->SetupAttachment(RootComponent);
    ExplosionEffect->bAutoActivate = false;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;

    bIsActive = false;
    TraveledDistance = 0.0f;

    Tags.Add(FName("Bullet"));
}

void ABoss_Attack1_Bullet::BeginPlay()
{
    Super::BeginPlay();

    // 보스 찾기
    BossRef = Cast<ABoss>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoss::StaticClass()));

    // Attack4와 동일하게 초기 스폰 시 BulletPool에 등록
    if (!BulletPool.Contains(this))
    {
        BulletPool.Add(this);
    }
}

void ABoss_Attack1_Bullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!bIsActive) return;

    TraveledDistance += ProjectileMovement->Velocity.Size() * DeltaTime;
    if (TraveledDistance >= MaxDistance)
    {
        ResetBullet();
    }
}

void ABoss_Attack1_Bullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Log, TEXT("OnHit: Triggered"));

    if (OtherActor)
    {
        FString TagList;
        for (const FName& Tag : OtherActor->Tags)
        {
            TagList += Tag.ToString() + TEXT(", ");
        }
        //UE_LOG(LogTemp, Log, TEXT("OnHit: OtherActor = %s, Tags = %s"), *OtherActor->GetName(), *TagList);
    }
    else
    {
        //UE_LOG(LogTemp, Log, TEXT("OnHit: OtherActor is null"));
    }

    bool bIsIgnored = false;
    if (!OtherActor || OtherActor == this || OtherActor->ActorHasTag("Boss") || OtherActor->ActorHasTag("Bullet"))
    {
        bIsIgnored = true;
    }
    //UE_LOG(LogTemp, Log, TEXT("OnHit: bIsIgnored = %s"), bIsIgnored ? TEXT("true") : TEXT("false"));

    if (bIsIgnored)
    {
        //UE_LOG(LogTemp, Log, TEXT("OnHit: Ignored collision; continuing flight without explosion"));
        return;  // 조건에 해당하면 후속 처리를 하지 않고 그대로 진행
    }

    //UE_LOG(LogTemp, Log, TEXT("OnHit: Applying damage"));
    ApplyDamage(OtherActor, BossRef ? BossRef->GetMonsterAttackDamage() : 10.0f);

    ACollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BulletMesh->SetVisibility(false);
    BCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ExplosionEffect->Activate(true);
    //UE_LOG(LogTemp, Log, TEXT("OnHit: ExplosionEffect activated"));

    GetWorld()->GetTimerManager().SetTimer(BCollisionDeactivateTimer, this, &ABoss_Attack1_Bullet::DeactivateBCollision, 0.5f, false);
}



void ABoss_Attack1_Bullet::ApplyDamage(AActor* Target, float DamageAmount)
{
    if (!Target) return;

    UGameplayStatics::ApplyDamage(Target, DamageAmount, nullptr, this, nullptr);
}

void ABoss_Attack1_Bullet::ApplyBCollisionDamage()
{
    // Collision A에서 오버랩된 액터 로그 출력
    TArray<AActor*> OverlapActorsA;
    ACollision->GetOverlappingActors(OverlapActorsA);
    for (AActor* Actor : OverlapActorsA)
    {
        //UE_LOG(LogTemp, Log, TEXT("Collision A Overlap: %s"), *Actor->GetName());
    }

    // Collision B에서 오버랩된 액터 로그 출력 및 데미지 적용
    TArray<AActor*> OverlapActorsB;
    BCollision->GetOverlappingActors(OverlapActorsB);
    for (AActor* Actor : OverlapActorsB)
    {
        //UE_LOG(LogTemp, Log, TEXT("Collision B Overlap: %s"), *Actor->GetName());
        ApplyDamage(Actor, BossRef ? BossRef->GetMonsterAttackDamage() : 5.0f);
    }
}


void ABoss_Attack1_Bullet::DeactivateBCollision()
{
    ApplyBCollisionDamage();
    BCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ResetBullet();
}

void ABoss_Attack1_Bullet::ResetBullet()
{
    //UE_LOG(LogTemp, Log, TEXT("ResetBullet: Begin resetting bullet state"));

    if (ProjectileMovement)
    {
        ProjectileMovement->StopMovementImmediately();
        ProjectileMovement->Deactivate();
        ProjectileMovement->Velocity = FVector::ZeroVector;
        //UE_LOG(LogTemp, Log, TEXT("ResetBullet: ProjectileMovement stopped, deactivated, and velocity reset"));
    }

    bIsActive = false;
    TraveledDistance = 0.0f;
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    //UE_LOG(LogTemp, Log, TEXT("ResetBullet: Actor hidden and collision disabled"));

    if (ACollision)
    {
        ACollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        //UE_LOG(LogTemp, Log, TEXT("ResetBullet: ACollision disabled"));
    }
    if (BCollision)
    {
        BCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        //UE_LOG(LogTemp, Log, TEXT("ResetBullet: BCollision disabled"));
    }
    if (BulletMesh)
    {
        BulletMesh->SetVisibility(false);
        //UE_LOG(LogTemp, Log, TEXT("ResetBullet: BulletMesh hidden"));
    }
    if (ExplosionEffect)
    {
        ExplosionEffect->Deactivate();
        //UE_LOG(LogTemp, Log, TEXT("ResetBullet: ExplosionEffect deactivated"));
    }

    if (!BulletPool.Contains(this))
    {
        BulletPool.Add(this);
        //UE_LOG(LogTemp, Log, TEXT("ResetBullet: Bullet added to pool"));
    }

    //UE_LOG(LogTemp, Log, TEXT("ResetBullet: Bullet state reset complete"));
}


ABoss_Attack1_Bullet* ABoss_Attack1_Bullet::GetBulletFromPool(UWorld* World, TSubclassOf<ABoss_Attack1_Bullet> BulletClass)
{
    if (BulletPool.Num() > 0)
    {
        ABoss_Attack1_Bullet* Bullet = BulletPool.Pop();
        if (Bullet)
        {
            Bullet->SetActorHiddenInGame(false);
            Bullet->SetActorEnableCollision(true);
            Bullet->bIsActive = true;
            return Bullet;
        }
    }

    // 풀에 총알이 없으면 새로 생성
    ABoss_Attack1_Bullet* NewBullet = World->SpawnActor<ABoss_Attack1_Bullet>(BulletClass);
    if (NewBullet)
    {
        NewBullet->bIsActive = true;
    }
    return NewBullet;
}

void ABoss_Attack1_Bullet::FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{
    //UE_LOG(LogTemp, Log, TEXT("FireProjectile: Called with SpawnLocation: %s, SpawnRotation: %s"),
        //*SpawnLocation.ToString(), *SpawnRotation.ToString());

    SetActorLocation(SpawnLocation);
    SetActorRotation(SpawnRotation);

    if (ProjectileMovement)
    {
        ProjectileMovement->StopMovementImmediately();
        ProjectileMovement->Activate(true);
        ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
        //UE_LOG(LogTemp, Log, TEXT("FireProjectile: ProjectileMovement activated with velocity: %s"),
            //*ProjectileMovement->Velocity.ToString());
    }
    else
    {
        //UE_LOG(LogTemp, Warning, TEXT("FireProjectile: ProjectileMovement is null"));
    }

    bIsActive = true;
    TraveledDistance = 0.0f;
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);

    if (ACollision)
    {
        ACollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        //UE_LOG(LogTemp, Log, TEXT("FireProjectile: ACollision set to QueryOnly"));
    }
    if (BulletMesh)
    {
        BulletMesh->SetVisibility(true);
        //UE_LOG(LogTemp, Log, TEXT("FireProjectile: BulletMesh set to visible"));
    }

    //UE_LOG(LogTemp, Log, TEXT("FireProjectile: Bullet is now active"));
}

